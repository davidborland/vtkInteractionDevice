/*=========================================================================

  Name:        vtkWiiMoteStyleCamera.cxx

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/

#include "vtkWiiMoteStyleCamera.h"

#include "vtkCamera.h"
#include "vtkInteractorObserver.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkVRPNAnalog.h"
#include "vtkVRPNAnalogOutput.h"
#include "vtkVRPNButton.h"

vtkStandardNewMacro(vtkWiiMoteStyleCamera);
vtkCxxRevisionMacro(vtkWiiMoteStyleCamera, "$Revision: 1.0 $");

//----------------------------------------------------------------------------
vtkWiiMoteStyleCamera::vtkWiiMoteStyleCamera() 
{ 
  this->ZoomSensitivity = 0.1;
  this->PanSensitivity = 0.05;
  this->RotateSensitivity = 10.0;

  this->XGravity = 0.0;
  this->YGravity = 0.0;
  this->ZGravity = 0.0;

  this->OldXGravity = 0.0;
  this->OldYGravity = 0.0;
  this->OldZGravity = 0.0;

  this->TriggerDown = false;
  this->HomeDown = false;
}

//----------------------------------------------------------------------------
vtkWiiMoteStyleCamera::~vtkWiiMoteStyleCamera() 
{
}

//----------------------------------------------------------------------------
void vtkWiiMoteStyleCamera::OnEvent(vtkObject* caller, unsigned long eid, void* callData) 
{
  // Need to initialize both here, as they cannot be initialized in the case statement
  vtkVRPNAnalog* analog = static_cast<vtkVRPNAnalog*>(caller);
  vtkVRPNButton* button = static_cast<vtkVRPNButton*>(caller);

  switch(eid)
    {
    case vtkVRPNDevice::AnalogEvent:
      this->OnAnalog(analog);
      break;

    case vtkVRPNDevice::ButtonEvent:
      this->OnButton(button);
      break;
    }
}

//----------------------------------------------------------------------------
void vtkWiiMoteStyleCamera::OnAnalog(vtkVRPNAnalog* analog)
{
  vtkCamera* camera = this->Renderer->GetActiveCamera();

  this->XGravity = analog->GetChannel(vtkWiiMoteStyle::GravityX);
  this->YGravity = analog->GetChannel(vtkWiiMoteStyle::GravityY);
  this->ZGravity = analog->GetChannel(vtkWiiMoteStyle::GravityZ);
}

//----------------------------------------------------------------------------
void vtkWiiMoteStyleCamera::OnButton(vtkVRPNButton* button)
{
  vtkCamera* camera = this->Renderer->GetActiveCamera();

  // Reset
  if (button->GetButton(vtkWiiMoteStyle::ButtonHome))
    {
    camera->SetPosition(0.0, 0.0, 1.0);
    camera->SetFocalPoint(0.0, 0.0, 0.0);
    camera->SetViewUp(0.0, 1.0, 0.0);
    this->Renderer->ResetCamera();

    if (this->AnalogOutput) this->AnalogOutput->SetChannel(0, 1.0);

    this->HomeDown = true;
    }
  else 
    {
    if (this->HomeDown)
      {
      if (this->AnalogOutput) this->AnalogOutput->SetChannel(0, 0.0);
      this->HomeDown = false;
      }
    }

  // Zoom
  if (button->GetButton(vtkWiiMoteStyle::ButtonMinus))
    {
    // Zoom out
    camera->Dolly(1.0 - this->ZoomSensitivity);
    } 
  else if (button->GetButton(vtkWiiMoteStyle::ButtonPlus))
    {
    // Zoom in
    camera->Dolly(1.0 + this->ZoomSensitivity);
    }

  // Pan
  if (button->GetButton(vtkWiiMoteStyle::ButtonLeft))
    {
    // Pan left
    this->Pan(-1.0, 0.0);
    }
  else if (button->GetButton(vtkWiiMoteStyle::ButtonRight))
    {
    // Pan right
    this->Pan(1.0, 0.0);
    }
  else if (button->GetButton(vtkWiiMoteStyle::ButtonDown))
    {
    // Pan down
    this->Pan(0.0, -1.0);
    }
  else if (button->GetButton(vtkWiiMoteStyle::ButtonUp))
    {
    // Pan up
    this->Pan(0.0, 1.0);
    }

  // Rotate
  if (button->GetButton(ButtonB))
    {
    // Rotate based on WiiMote orientation
    if (!this->TriggerDown)
      {
      this->OldXGravity = this->XGravity;
      this->OldYGravity = this->YGravity;
      this->OldZGravity = this->ZGravity;
      }

    camera->Azimuth((this->XGravity - this->OldXGravity) * this->RotateSensitivity);
    camera->Elevation(-(this->YGravity - this->OldYGravity) * this->RotateSensitivity);
//    camera->Roll(-(this->ZGravity - this->OldZGravity) * this->RotateSensitivity);
    camera->OrthogonalizeViewUp();

    this->TriggerDown = true;
    }
  else
    {
    this->TriggerDown = false;
    }

  this->Renderer->ResetCameraClippingRange();
  // Render() will be called in the interactor
}

//----------------------------------------------------------------------------
void vtkWiiMoteStyleCamera::Pan(double xDelta, double yDelta)
{
  vtkCamera* camera = this->Renderer->GetActiveCamera();

  double viewFocus[4], focalDepth, viewPoint[3];
  double newPickPoint[4], oldPickPoint[4], motionVector[3];

  camera->GetFocalPoint(viewFocus);
  vtkInteractorObserver::ComputeWorldToDisplay(this->Renderer, 
                                               viewFocus[0], viewFocus[1], viewFocus[2], 
                                               viewFocus);
  focalDepth = viewFocus[2];

  // Normalize for window size
  int width = this->Renderer->GetRenderWindow()->GetSize()[0];
  int height = this->Renderer->GetRenderWindow()->GetSize()[1];
  xDelta *= width * this->PanSensitivity;
  yDelta *= height * this->PanSensitivity;

  vtkInteractorObserver::ComputeDisplayToWorld(this->Renderer, 
                                               xDelta, yDelta, focalDepth, 
                                               newPickPoint);

  vtkInteractorObserver::ComputeDisplayToWorld(this->Renderer, 
                                               0, 0, focalDepth, 
                                               oldPickPoint);

  // Camera motion is reversed
  motionVector[0] = newPickPoint[0] - oldPickPoint[0];
  motionVector[1] = newPickPoint[1] - oldPickPoint[1];
  motionVector[2] = newPickPoint[2] - oldPickPoint[2];

  camera->GetFocalPoint(viewFocus);
  camera->GetPosition(viewPoint);
  camera->SetFocalPoint(motionVector[0] + viewFocus[0],
                        motionVector[1] + viewFocus[1],
                        motionVector[2] + viewFocus[2]);

  camera->SetPosition(motionVector[0] + viewPoint[0],
                      motionVector[1] + viewPoint[1],
                      motionVector[2] + viewPoint[2]);
}

//----------------------------------------------------------------------------
void vtkWiiMoteStyleCamera::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "ZoomSensitivity: " << this->ZoomSensitivity << "\n";
  os << indent << "PanSensitivity: " << this->PanSensitivity << "\n";
  os << indent << "RotateSensitivity: " << this->RotateSensitivity << "\n";
  os << indent << "XGravity: " << this->XGravity << "\n";
  os << indent << "YGravity: " << this->YGravity << "\n";
  os << indent << "ZGravity: " << this->ZGravity << "\n";
  os << indent << "OldXGravity: " << this->OldXGravity << "\n";
  os << indent << "OldYGravity: " << this->OldYGravity << "\n";
  os << indent << "OldZGravity: " << this->OldZGravity << "\n";
  os << indent << "TriggerDown: " << this->TriggerDown << "\n";
  os << indent << "HomeDown: " << this->HomeDown << "\n";
}
/*=========================================================================

  Name:        vtkRenciMultiTouchStyleCamera.cxx

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/

#include "vtkRenciMultiTouchStyleCamera.h"

#include "vtkCamera.h"
#include "vtkInteractorObserver.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkRenciMultiTouch.h"
#include "vtkRenderer.h"
#include "vtkstd/string"
#include "vtkstd/vector"

vtkStandardNewMacro(vtkRenciMultiTouchStyleCamera);
vtkCxxRevisionMacro(vtkRenciMultiTouchStyleCamera, "$Revision: 1.0 $");

//----------------------------------------------------------------------------
vtkRenciMultiTouchStyleCamera::vtkRenciMultiTouchStyleCamera() 
{ 
}

//----------------------------------------------------------------------------
vtkRenciMultiTouchStyleCamera::~vtkRenciMultiTouchStyleCamera() 
{
}

//----------------------------------------------------------------------------
void vtkRenciMultiTouchStyleCamera::OnEvent(vtkObject* caller, unsigned long eid, void* calldata) 
{
  vtkRenciMultiTouch* multiTouch = static_cast<vtkRenciMultiTouch*>(caller);

  switch(eid)
    {
    // One Drag
    case vtkRenciMultiTouch::OneDragEvent:
      this->OnOneDrag(multiTouch);
      break;

    // Zoom
    case vtkRenciMultiTouch::ZoomEvent:
      this->OnZoom(multiTouch);
      break;

    // Translate X
    case vtkRenciMultiTouch::TranslateXEvent:
      this->OnTranslateX(multiTouch);
      break;

    // Translate Y
    case vtkRenciMultiTouch::TranslateYEvent:
      this->OnTranslateY(multiTouch);
      break;

    // Rotate X
    case vtkRenciMultiTouch::RotateXEvent:
      this->OnRotateX(multiTouch);
      break;

    // Rotate Y
    case vtkRenciMultiTouch::RotateYEvent:
      this->OnRotateY(multiTouch);
      break;

    // Rotate Z
    case vtkRenciMultiTouch::RotateZEvent:
      this->OnRotateZ(multiTouch);
      break;
    }
}

//----------------------------------------------------------------------------
void vtkRenciMultiTouchStyleCamera::OnOneDrag(vtkRenciMultiTouch* multiTouch)
{
  vtkCamera* camera = this->Renderer->GetActiveCamera();

  int numTouches = multiTouch->GetNumberOfTouchPoints();
  if (numTouches < 1) return;

  vtkstd::vector<TouchPoint> touches;
  touches.resize(numTouches);
  for (int i = 0; i < numTouches; i++) 
    {
    touches[i] = multiTouch->GetTouchPoint(i);
    }

  // XXX: Magic number
  double rotateSensitivity = 500.0;
  double dx = touches[0].Direction[0] * rotateSensitivity;
  double dy = touches[0].Direction[1] * rotateSensitivity; 

  camera->Azimuth(-dx);
  camera->Elevation(dy);
  camera->OrthogonalizeViewUp();

  this->Renderer->ResetCameraClippingRange();
  // Render() will be called in the interactor
}

//----------------------------------------------------------------------------
void vtkRenciMultiTouchStyleCamera::OnZoom(vtkRenciMultiTouch* multiTouch)
{
  vtkCamera* camera = this->Renderer->GetActiveCamera();

  int numTouches = multiTouch->GetNumberOfTouchPoints();
  if (numTouches < 2) return;

  vtkstd::vector<TouchPoint> touches;
  touches.resize(numTouches);
  for (int i = 0; i < numTouches; i++) 
    {
    touches[i] = multiTouch->GetTouchPoint(i);
    }

  double zoomAmount = sqrt(touches[0].Direction[0] * touches[0].Direction[0] +
                           touches[0].Direction[1] * touches[0].Direction[1]);

  // Find right-most touch
  if (touches[0].Location[0] > touches[1].Location[0])
    {
    // Touch 0 is right-most
    if (touches[0].Direction[0] < 0)
      {
      // Right-most touch moving left == zoom out
      zoomAmount = -zoomAmount;
      }
    }
  else
    {
    // Touch 1 is right-most
    if (touches[1].Direction[0] < 0)
      // Right-most touch moving left == zoom out
      {
      zoomAmount = -zoomAmount;
      }
    }

  double zoomScale = 20.0;
  zoomAmount = pow(1.1, zoomAmount * zoomScale);

  // Zoom the camera
  if (camera->GetParallelProjection())
    {
    camera->SetParallelScale(camera->GetParallelScale() / zoomAmount);
    }
  else
    {
    camera->Dolly(zoomAmount);
    }

  this->Renderer->ResetCameraClippingRange();
  // Render() will be called in the interactor
}

//----------------------------------------------------------------------------
void vtkRenciMultiTouchStyleCamera::OnTranslateX(vtkRenciMultiTouch* multiTouch)
{
  vtkCamera* camera = this->Renderer->GetActiveCamera();

  int numTouches = multiTouch->GetNumberOfTouchPoints();

  vtkstd::vector<TouchPoint> touches;
  touches.resize(numTouches);
  for (int i = 0; i < numTouches; i++) 
    {
    touches[i] = multiTouch->GetTouchPoint(i);
    }

  int i;
  for (i = 0; i < (int)touches.size(); i++) 
    {
    if (touches[i].MoveLocation != 0) break;
    }
  if (i >= (int)touches.size()) return;

  // XXX: Magic number
  double translateSensitivity = 1000.0;
  double x = touches[i].Location[0];
  double y = touches[i].Location[1];
  double dx = touches[i].Direction[1] * translateSensitivity;
  double dy = 0.0; 

  double viewFocus[4], focalDepth, viewPoint[3];
  double newPickPoint[4], oldPickPoint[4], motionVector[3];

  camera->GetFocalPoint(viewFocus);
  vtkInteractorObserver::ComputeWorldToDisplay(this->Renderer, 
                                               viewFocus[0], viewFocus[1], viewFocus[2], 
                                               viewFocus);
  focalDepth = viewFocus[2];

  vtkInteractorObserver::ComputeWorldToDisplay(this->Renderer, 
                                               x, y, focalDepth, 
                                               newPickPoint);

  vtkInteractorObserver::ComputeWorldToDisplay(this->Renderer,
                                               x + dx, y + dy, focalDepth, 
                                               oldPickPoint);
  
  // Camera motion is reversed
  motionVector[0] = oldPickPoint[0] - newPickPoint[0];
  motionVector[1] = oldPickPoint[1] - newPickPoint[1];
  motionVector[2] = oldPickPoint[2] - newPickPoint[2];
  
  camera->GetFocalPoint(viewFocus);
  camera->GetPosition(viewPoint);
  camera->SetFocalPoint(motionVector[0] + viewFocus[0],
                        motionVector[1] + viewFocus[1],
                        motionVector[2] + viewFocus[2]);

  camera->SetPosition(motionVector[0] + viewPoint[0],
                      motionVector[1] + viewPoint[1],
                      motionVector[2] + viewPoint[2]);
      
  // Render() will be called in the interactor
}

//----------------------------------------------------------------------------
void vtkRenciMultiTouchStyleCamera::OnTranslateY(vtkRenciMultiTouch* multiTouch)
{
  vtkCamera* camera = this->Renderer->GetActiveCamera();

  int numTouches = multiTouch->GetNumberOfTouchPoints();

  vtkstd::vector<TouchPoint> touches;
  touches.resize(numTouches);
  for (int i = 0; i < numTouches; i++) 
    {
    touches[i] = multiTouch->GetTouchPoint(i);
    }

  int i;
  for (i = 0; i < (int)touches.size(); i++) 
    {
    if (touches[i].MoveLocation != 0) break;
    }
  if (i >= (int)touches.size()) return;

  double translateScale = 1000.0;
  double x = touches[i].Location[0];
  double y = touches[i].Location[1];
  double dx = 0.0;
  double dy = touches[i].Direction[1] * translateScale; 

  double viewFocus[4], focalDepth, viewPoint[3];
  double newPickPoint[4], oldPickPoint[4], motionVector[3];

  camera->GetFocalPoint(viewFocus);
  vtkInteractorObserver::ComputeWorldToDisplay(this->Renderer, 
                                               viewFocus[0], viewFocus[1], viewFocus[2], 
                                               viewFocus);
  focalDepth = viewFocus[2];

  vtkInteractorObserver::ComputeWorldToDisplay(this->Renderer, 
                                               x, y, focalDepth, 
                                               newPickPoint);

  vtkInteractorObserver::ComputeWorldToDisplay(this->Renderer, 
                                               x + dx, y + dy, focalDepth, 
                                               oldPickPoint);
  
  // Camera motion is reversed
  motionVector[0] = oldPickPoint[0] - newPickPoint[0];
  motionVector[1] = oldPickPoint[1] - newPickPoint[1];
  motionVector[2] = oldPickPoint[2] - newPickPoint[2];
  
  camera->GetFocalPoint(viewFocus);
  camera->GetPosition(viewPoint);
  camera->SetFocalPoint(motionVector[0] + viewFocus[0],
                        motionVector[1] + viewFocus[1],
                        motionVector[2] + viewFocus[2]);

  camera->SetPosition(motionVector[0] + viewPoint[0],
                      motionVector[1] + viewPoint[1],
                      motionVector[2] + viewPoint[2]);
      
  // Render() will be called in the interactor
}

//----------------------------------------------------------------------------
void vtkRenciMultiTouchStyleCamera::OnRotateX(vtkRenciMultiTouch* multiTouch)
{
  vtkCamera* camera = this->Renderer->GetActiveCamera();

  int numTouches = multiTouch->GetNumberOfTouchPoints();

  vtkstd::vector<TouchPoint> touches;
  touches.resize(numTouches);
  for (int i = 0; i < numTouches; i++) 
    {
    touches[i] = multiTouch->GetTouchPoint(i);
    }

  int i;
  for (i = 0; i < (int)touches.size(); i++) 
    {
    if (touches[i].MoveLocation != 0) break;
    }
  if (i >= (int)touches.size()) return;

  double rotateScale = 500.0;
  double dy = touches[i].Direction[1] * rotateScale; 

  camera->Elevation(dy);
  camera->OrthogonalizeViewUp();

  this->Renderer->ResetCameraClippingRange();
  // Render() will be called in the interactor
}

//----------------------------------------------------------------------------
void vtkRenciMultiTouchStyleCamera::OnRotateY(vtkRenciMultiTouch* multiTouch)
{
  vtkCamera* camera = this->Renderer->GetActiveCamera();  
  
  int numTouches = multiTouch->GetNumberOfTouchPoints();

  vtkstd::vector<TouchPoint> touches;
  touches.resize(numTouches);
  for (int i = 0; i < numTouches; i++) 
    {
    touches[i] = multiTouch->GetTouchPoint(i);
    }

  int i;
  for (i = 0; i < (int)touches.size(); i++) 
    {
    if (touches[i].MoveLocation != 0) break;
    }
  if (i >= (int)touches.size()) return;

  double rotateScale = 500.0;
  double dy = touches[i].Direction[1] * rotateScale; 

  camera->Azimuth(-dy);
  camera->OrthogonalizeViewUp();

  this->Renderer->ResetCameraClippingRange();
  // Render() will be called in the interactor
}

//----------------------------------------------------------------------------
void vtkRenciMultiTouchStyleCamera::OnRotateZ(vtkRenciMultiTouch* multiTouch)
{
  vtkCamera* camera = this->Renderer->GetActiveCamera();

  int numTouches = multiTouch->GetNumberOfTouchPoints();

  vtkstd::vector<TouchPoint> touches;
  touches.resize(numTouches);
  for (int i = 0; i < numTouches; i++) 
    {
    touches[i] = multiTouch->GetTouchPoint(i);
    }

  int i;
  for (i = 0; i < (int)touches.size(); i++) 
    {
    if (touches[i].MoveLocation != 0) break;
    }
  if (i >= (int)touches.size()) return;

  double rotateScale = 500.0;
  double dy = touches[i].Direction[1] * 500.0; 

  camera->Roll(-dy);
  camera->OrthogonalizeViewUp();

  this->Renderer->ResetCameraClippingRange();
  // Render() will be called in the interactor
}

//----------------------------------------------------------------------------
void vtkRenciMultiTouchStyleCamera::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
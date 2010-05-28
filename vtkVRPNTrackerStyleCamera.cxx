/*=========================================================================

  Name:        vtkVRPNTrackerStyleCamera.cxx

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/

#include "vtkVRPNTrackerStyleCamera.h"

#include "vtkCallbackCommand.h"
#include "vtkCamera.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkVRPNTracker.h"

vtkStandardNewMacro(vtkVRPNTrackerStyleCamera);
vtkCxxRevisionMacro(vtkVRPNTrackerStyleCamera, "$Revision: 1.0 $");

//----------------------------------------------------------------------------
vtkVRPNTrackerStyleCamera::vtkVRPNTrackerStyleCamera() 
{ 
}

//----------------------------------------------------------------------------
vtkVRPNTrackerStyleCamera::~vtkVRPNTrackerStyleCamera() 
{
}

//----------------------------------------------------------------------------
void vtkVRPNTrackerStyleCamera::OnEvent(vtkObject* caller, unsigned long eid, void* callData) 
{
  vtkVRPNTracker* tracker = static_cast<vtkVRPNTracker*>(caller);

  switch(eid)
    {
    case vtkVRPNDevice::TrackerEvent:
      this->OnTracker(tracker);
      break;
    }
}

//----------------------------------------------------------------------------
void vtkVRPNTrackerStyleCamera::SetTracker(vtkVRPNTracker* tracker)
{  
  if (tracker != NULL) 
    {
    tracker->AddObserver(vtkVRPNDevice::TrackerEvent, this->DeviceCallback);
    }
} 

//----------------------------------------------------------------------------
void vtkVRPNTrackerStyleCamera::OnTracker(vtkVRPNTracker* tracker)
{
  vtkCamera* camera = this->Renderer->GetActiveCamera();

  // Get the transformed position
  double position[3];
  for (int i = 0; i < 3; i++) 
    {
    position[i] = tracker->GetPosition()[i] + tracker->GetTracker2RoomTranslation()[i];
    }

  // Get the rotation matrix
  double rotation[3][3];
  vtkMath::QuaternionToMatrix3x3(tracker->GetRotation(), rotation);

  // Transform the rotation
  double t2rRotation[3][3];
  vtkMath::QuaternionToMatrix3x3(tracker->GetTracker2RoomRotation(), t2rRotation);
  vtkMath::Multiply3x3(rotation, t2rRotation, rotation);

  // Calculate the view direction
  double forward[3] = { 0.0, 0.0, 1.0 };
  vtkMath::Multiply3x3(rotation, forward, forward);
  for (int i = 0; i < 3; i++) forward[i] += tracker->GetPosition()[i];

  // Calculate the up vector
  double up[3] = { 0.0, 1.0, 0.0 };
  vtkMath::Multiply3x3(rotation, up, up);

  // Set camera parameters
  camera->SetPosition(position);
  camera->SetFocalPoint(forward);
  camera->SetViewUp(up);
  camera->Modified();

  // Render
  this->Renderer->ResetCameraClippingRange();
  // Render() will be called in the interactor
}

//----------------------------------------------------------------------------
void vtkVRPNTrackerStyleCamera::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
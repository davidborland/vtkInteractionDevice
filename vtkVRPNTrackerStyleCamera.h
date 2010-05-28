/*=========================================================================

  Name:        vtkVRPNTrackerStyleCamera.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/
// .NAME vtkVRPNTrackerStyleCamera
// .SECTION Description
// vtkVRPNTrackerStyleCamera moves the camera based on tracker events 
// generated by devices using the Virtual Reality Peripheral Network 
// (VRPN: http://www.cs.unc.edu/Research/vrpn/).  

// .SECTION see also
// vtkDeviceInteractor vtkInteractionDevice

#ifndef __vtkVRPNTrackerStyleCamera_h
#define __vtkVRPNTrackerStyleCamera_h

#include "vtkInteractionDeviceConfigure.h"

#include "vtkDeviceInteractorStyle.h"

class vtkVRPNTracker;

class VTK_INTERACTIONDEVICE_EXPORT vtkVRPNTrackerStyleCamera : public vtkDeviceInteractorStyle
{
public:
  static vtkVRPNTrackerStyleCamera* New();
  vtkTypeRevisionMacro(vtkVRPNTrackerStyleCamera,vtkDeviceInteractorStyle);
  void PrintSelf(ostream&, vtkIndent); 

  // Description:
  // Perform interaction based on an event
  virtual void OnEvent(vtkObject* caller, unsigned long eid, void* callData);

  // Description:
  // Set the tracker receiving events from
  void SetTracker(vtkVRPNTracker*);

protected:
  vtkVRPNTrackerStyleCamera();
  ~vtkVRPNTrackerStyleCamera();

  virtual void OnTracker(vtkVRPNTracker*);

private:
  vtkVRPNTrackerStyleCamera(const vtkVRPNTrackerStyleCamera&);  // Not implemented.
  void operator=(const vtkVRPNTrackerStyleCamera&);  // Not implemented.
};

#endif
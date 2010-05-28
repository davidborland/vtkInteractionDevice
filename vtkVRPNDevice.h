/*=========================================================================

  Name:        vtkVRPNDevice.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/
// .NAME vtkVRPNDevice
// .SECTION Description
// vtkVRPNDevice is an abstract base class for interfacing with external 
// devices using the Virtual Reality Peripheral Network (VRPN: 
// http://www.cs.unc.edu/Research/vrpn/).  

// .SECTION see also
// vtkInteractionDeviceManager vtkDeviceInteractorStyle

#ifndef __vtkVRPNDevice_h
#define __vtkVRPNDevice_h

#include "vtkInteractionDeviceConfigure.h"

#include "vtkInteractionDevice.h"

#include "vtkCommand.h"

class VTK_INTERACTIONDEVICE_EXPORT vtkVRPNDevice : public vtkInteractionDevice
{
public:
  vtkTypeRevisionMacro(vtkVRPNDevice,vtkInteractionDevice);
  void PrintSelf(ostream&, vtkIndent);

  // Description:
  // Set the name of the device to connect to.  Must be set before Initialize().
  vtkSetStringMacro(DeviceName);

  // Enumeration for VRPN events
  //BTX
  enum VRPNEventIds {
      AnalogEvent = vtkCommand::UserEvent,
      ButtonEvent,
      TrackerEvent
  };
  //ETX

protected:
  vtkVRPNDevice();
  ~vtkVRPNDevice();

  char* DeviceName;

private:
  vtkVRPNDevice(const vtkVRPNDevice&);  // Not implemented.
  void operator=(const vtkVRPNDevice&);  // Not implemented.
};

#endif

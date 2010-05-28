/*=========================================================================

  Name:        vtkInteractionDevice.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/
// .NAME vtkInteractionDevice
// .SECTION Description
// vtkInteractionDevice is an abstract base class for interfacing with
// external devices for interaction.  Examples include multi-touch 
// interfaces and various devices supported by the Virtual Reality 
// Peripheral Network (VRPN: http://www.cs.unc.edu/Research/vrpn/).  

// .SECTION see also
// vtkInteractionDeviceManager vtkDeviceInteractorStyle

#ifndef __vtkInteractionDevice_h
#define __vtkInteractionDevice_h

#include "vtkInteractionDeviceConfigure.h"

#include "vtkObject.h"

class VTK_INTERACTIONDEVICE_EXPORT vtkInteractionDevice : public vtkObject
{
public:
  vtkTypeRevisionMacro(vtkInteractionDevice,vtkObject);
  void PrintSelf(ostream&, vtkIndent);

  // Description:
  // Initialize the device
  virtual int Initialize() = 0;

  // Description:
  // Receive updates from the device
  virtual void Update() = 0;

  // Description:
  // Invoke the appropriate event for observers to listen for
  virtual void InvokeInteractionEvent() = 0;

protected:
  vtkInteractionDevice();
  ~vtkInteractionDevice();

private:
  vtkInteractionDevice(const vtkInteractionDevice&);  // Not implemented.
  void operator=(const vtkInteractionDevice&);  // Not implemented.
};

#endif



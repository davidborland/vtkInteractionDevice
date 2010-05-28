/*=========================================================================

  Name:        vtkVRPNAnalog.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/
// .NAME vtkVRPNAnalog
// .SECTION Description
// vtkVRPNAnalog interfaces with an analog device using the Virtual 
// Reality Peripheral Network (VRPN: 
// http://www.cs.unc.edu/Research/vrpn/).  

// .SECTION see also
// vtkInteractionDeviceManager vtkDeviceInteractorStyle

#ifndef __vtkVRPNAnalog_h
#define __vtkVRPNAnalog_h

#include "vtkInteractionDeviceConfigure.h"

#include "vtkVRPNDevice.h"

class vrpn_Analog_Remote;

// Holds vtkstd member variables, which must be hidden
class vtkVRPNAnalogInternals;

class VTK_INTERACTIONDEVICE_EXPORT vtkVRPNAnalog : public vtkVRPNDevice
{
public:
  static vtkVRPNAnalog* New();
  vtkTypeRevisionMacro(vtkVRPNAnalog,vtkVRPNDevice);
  void PrintSelf(ostream&, vtkIndent);

  // Description:
  // Initialize the device
  virtual int Initialize();

  // Description:
  // Receive updates from the device
  virtual void Update();

  // Description:
  // Invoke vrpnDevice::AnalogEvent for observers to listen for
  virtual void InvokeInteractionEvent();

  // Description:
  // The number of channels to use
  void SetNumberOfChannels(int num);
  int GetNumberOfChannels();

  // Description:
  // Set/Get the analog information
  void SetChannel(int channel, double value);
  double GetChannel(int channel);

protected:
  vtkVRPNAnalog();
  ~vtkVRPNAnalog();

  vrpn_Analog_Remote* Analog;

  vtkVRPNAnalogInternals* Internals;

private:
  vtkVRPNAnalog(const vtkVRPNAnalog&);  // Not implemented.
  void operator=(const vtkVRPNAnalog&);  // Not implemented.
};

#endif

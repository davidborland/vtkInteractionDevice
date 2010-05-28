/*=========================================================================

  Name:        vtkVRPNAnalogOutput.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/
// .NAME vtkVRPNAnalogOutput
// .SECTION Description
// vtkVRPNAnalogOutput interfaces with an analog output device using the 
// Virtual Reality Peripheral Network (VRPN: 
// http://www.cs.unc.edu/Research/vrpn/).  

// .SECTION see also
// vtkInteractionDeviceManager vtkDeviceInteractorStyle

#ifndef __vtkVRPNAnalogOutput_h
#define __vtkVRPNAnalogOutput_h

#include "vtkInteractionDeviceConfigure.h"

#include "vtkVRPNDevice.h"

class vrpn_Analog_Output_Remote;

// Holds vtkstd member variables, which must be hidden
class vtkVRPNAnalogOutputInternals;

class VTK_INTERACTIONDEVICE_EXPORT vtkVRPNAnalogOutput : public vtkVRPNDevice
{
public:
  static vtkVRPNAnalogOutput* New();
  vtkTypeRevisionMacro(vtkVRPNAnalogOutput,vtkVRPNDevice);
  void PrintSelf(ostream&, vtkIndent);

  // Description:
  // Initialize the device
  virtual int Initialize();

  // Description:
  // Receive updates from the device
  virtual void Update();

  // Description:
  // No event 
  virtual void InvokeInteractionEvent() {}

  // Description:
  // Set the analog information
  void SetChannel(int channel, double value);

protected:
  vtkVRPNAnalogOutput();
  ~vtkVRPNAnalogOutput();

  vrpn_Analog_Output_Remote* AnalogOutput;

private:
  vtkVRPNAnalogOutput(const vtkVRPNAnalogOutput&);  // Not implemented.
  void operator=(const vtkVRPNAnalogOutput&);  // Not implemented.
};

#endif

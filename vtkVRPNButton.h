/*=========================================================================

  Name:        vtkVRPNButton.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/
// .NAME vtkVRPNButton
// .SECTION Description
// vtkVRPNButton interfaces with a button device using the Virtual 
// Reality Peripheral Network (VRPN: 
// http://www.cs.unc.edu/Research/vrpn/).  

// .SECTION see also
// vtkInteractionDeviceManager vtkDeviceInteractorStyle

#ifndef __vtkVRPNButton_h
#define __vtkVRPNButton_h

#include "vtkInteractionDeviceConfigure.h"

#include "vtkVRPNDevice.h"

class vrpn_Button_Remote;

// Holds vtkstd member variables, which must be hidden
class vtkVRPNButtonInternals;

class VTK_INTERACTIONDEVICE_EXPORT vtkVRPNButton : public vtkVRPNDevice
{
public:
  static vtkVRPNButton* New();
  vtkTypeRevisionMacro(vtkVRPNButton,vtkVRPNDevice);
  void PrintSelf(ostream&, vtkIndent);

  // Description:
  // Initialize the device
  virtual int Initialize();

  // Description:
  // Receive updates from the device
  virtual void Update();

  // Description:
  // Invoke vrpnDevice::ButtonEvent for observers to listen for
  virtual void InvokeInteractionEvent();

  // Description:
  // The number of buttons to use
  void SetNumberOfButtons(int num);
  int GetNumberOfButtons();

  // Description:
  // Set/Get the button information
  void SetButton(int button, bool value);
  bool GetButton(int button);

  // Description:
  // Use toggle buttons or not.  Will have no effect until the device is initialized.
  void SetToggle(int button, bool toggle);

protected:
  vtkVRPNButton();
  ~vtkVRPNButton();

  vrpn_Button_Remote* Button;

  vtkVRPNButtonInternals* Internals;

private:
  vtkVRPNButton(const vtkVRPNButton&);  // Not implemented.
  void operator=(const vtkVRPNButton&);  // Not implemented.
};

#endif

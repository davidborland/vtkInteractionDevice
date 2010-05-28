/*=========================================================================

  Name:        vtkWiiMoteStyle.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/
// .NAME vtkWiiMoteStyle
// .SECTION Description
// vtkWiiMoteStyle is an abstract base class for 
// vtkDeviceInteractorStyles that interface with a WiiMote using the 
// Virtual Reality Peripheral Network (VRPN: 
// http://www.cs.unc.edu/Research/vrpn/).  
//
// .SECTION see also
// vtkDeviceInteractor vtkInteractionDevice

#ifndef __vtkWiiMoteStyle_h
#define __vtkWiiMoteStyle_h

#include "vtkInteractionDeviceConfigure.h"

#include "vtkDeviceInteractorStyle.h"

class vtkVRPNAnalog;
class vtkVRPNAnalogOutput;
class vtkVRPNButton;

class VTK_INTERACTIONDEVICE_EXPORT vtkWiiMoteStyle : public vtkDeviceInteractorStyle
{
public:
  vtkTypeRevisionMacro(vtkWiiMoteStyle,vtkDeviceInteractorStyle);
  void PrintSelf(ostream&, vtkIndent); 

  // Description:
  // Set the devices to connect to the WiiMote
  void SetAnalog(vtkVRPNAnalog*);
  void SetButton(vtkVRPNButton*);
  void SetAnalogOutput(vtkVRPNAnalogOutput*);

protected:
  vtkWiiMoteStyle();
  ~vtkWiiMoteStyle();

  // Enumeration for buttons:
  //   button[0] = 2
  //   button[1] = 1
  //   button[2] = B (Trigger)
  //   button[3] = A
  //   button[4] = -
  //   button[5] = 
  //   button[6] = 
  //   button[7] = HOME
  //   button[8] = D-pad left
  //   button[9] = D-pad right
  //   button[10] = D-pad down
  //   button[11] = D-pad up
  //   button[12] = +
  //   button[13] = 
  //   button[14] = 
  //   button[15] = 
  //BTX
  enum WiiMoteButtons {
    Button2 = 0,
    Button1,
    ButtonB,
    ButtonA,
    ButtonMinus,
    ButtonHome = 7,
    ButtonLeft,
    ButtonRight,
    ButtonDown,
    ButtonUp,
    ButtonPlus
  };
  //ETX

  // Enumeration for analog channels:
  //   channel[0] = Battery level (0-1)
  //   channel[1] = Gravity X vector calculation (1 = Earth gravity)
  //   channel[2] = Gravity Y vector calculation (1 = Earth gravity)
  //   channel[3] = Gravity Z vector calculation (1 = Earth gravity)
  //   channel[4] = X of first sensor spot (0-1023, -1 if not seen)
  //   channel[5] = Y of first sensor spot (0-767, -1 if not seen)
  //   channel[6] = Size of first sensor spot (0-15, -1 if not seen)
  //   channel[7] = X of second sensor spot (0-1023, -1 if not seen)
  //   channel[9] = Y of second sensor spot (0-767, -1 if not seen)
  //   channel[9] = Size of second sensor spot (0-15, -1 if not seen)
  //   channel[10] = X of third sensor spot (0-1023, -1 if not seen)
  //   channel[11] = Y of third sensor spot (0-767, -1 if not seen)
  //   channel[12] = Size of third sensor spot (0-15, -1 if not seen)
  //   channel[13] = X of fourth sensor spot (0-1023, -1 if not seen)
  //   channel[14] = Y of fourth sensor spot (0-767, -1 if not seen)
  //   channel[15] = Size of fourth sensor spot (0-15, -1 if not seen)
  //BTX
  enum WiiMoteAnalogChannels {
    Battery = 0,
    GravityX,
    GravityY,
    GravityZ,
    Sensor1X,
    Sensor1Y,
    Sensor1Size,
    Sensor2X,
    Sensor2Y,
    Sensor2Size,
    Sensor3X,
    Sensor3Y,
    Sensor3Size,
    Sensor4X,
    Sensor4Y,
    Sensor4Size
  };
  //ETX

  virtual void OnAnalog(vtkVRPNAnalog*) = 0;
  virtual void OnButton(vtkVRPNButton*) = 0;

  // Need to hold a reference to the analog output device, which is used
  // to control the WiiMote buzzer, as we request output instead of 
  // registering callbacks for input from the device.
  vtkVRPNAnalogOutput* AnalogOutput;

private:
  vtkWiiMoteStyle(const vtkWiiMoteStyle&);  // Not implemented.
  void operator=(const vtkWiiMoteStyle&);  // Not implemented.
};

#endif
/*=========================================================================

  Name:        vtkDeviceInteractorStyle.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/
// .NAME vtkDeviceInteractorStyle
// .SECTION Description
// vtkDeviceInteractorStyle is an abstract base class to define different
// interaction styles for interfacing with external devices.  Examples of
// such devices include multi-touch interfaces and various devices 
// supported by the Virtual Reality Peripheral Network (VRPN: 
// http://www.cs.unc.edu/Research/vrpn/).  

// .SECTION see also
// vtkDeviceInteractor vtkInteractionDevice

#ifndef __vtkDeviceInteractorStyle_h
#define __vtkDeviceInteractorStyle_h

#include "vtkInteractionDeviceConfigure.h"

#include "vtkObject.h"

class vtkCallbackCommand;
class vtkRenderer;

class VTK_INTERACTIONDEVICE_EXPORT vtkDeviceInteractorStyle : public vtkObject
{
public:
  vtkTypeRevisionMacro(vtkDeviceInteractorStyle,vtkObject);
  void PrintSelf(ostream&, vtkIndent); 

  // Description:
  // Perform interaction based on an event
  virtual void OnEvent(vtkObject* caller, unsigned long eid, void* callData) = 0;

  // Description:
  // Set the renderer being used
  void SetRenderer(vtkRenderer*);

protected:
  vtkDeviceInteractorStyle();
  ~vtkDeviceInteractorStyle();

  vtkRenderer* Renderer;
  
  vtkCallbackCommand* DeviceCallback;

  // Description:
  // Calls the OnEvent() method to act on subclasses 
  static void ProcessEvents(vtkObject* caller, 
                            unsigned long eid,
                            void* clientdata, 
                            void* calldata);

private:
  vtkDeviceInteractorStyle(const vtkDeviceInteractorStyle&);  // Not implemented.
  void operator=(const vtkDeviceInteractorStyle&);  // Not implemented.
};

#endif
/*=========================================================================

  Name:        vtkWin32RenderWindowDeviceInteractor.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/
// .NAME vtkWin32RenderWindowDeviceInteractor
// .SECTION Description
// vtkWin32RenderWindowDeviceInteractor adds interaction with external devices to
// vtkWin32RenderWindowInteractor.  Examples of such devices include
// multi-touch interfaces and various devices supported by the Virtual 
// Reality Peripheral Network (VRPN: 
// http://www.cs.unc.edu/Research/vrpn/).  

// .SECTION see also
// vtkInteractionDeviceManager vtkInteractionDevice 
// vtkDeviceInteractorStyle

#ifndef __vtkWin32RenderWindowDeviceInteractor_h
#define __vtkWin32RenderWindowDeviceInteractor_h

#include "vtkInteractionDeviceConfigure.h"

#include "vtkWin32RenderWindowInteractor.h"

class vtkDeviceInteractor;

class VTK_INTERACTIONDEVICE_EXPORT vtkWin32RenderWindowDeviceInteractor : public vtkWin32RenderWindowInteractor
{
public:
  static vtkWin32RenderWindowDeviceInteractor* New();
  vtkTypeRevisionMacro(vtkWin32RenderWindowDeviceInteractor,vtkWin32RenderWindowInteractor);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // This will start up the event loop and never return. If you
  // call this method it will loop processing events until the
  // application is exited.
  virtual void Start();

  // Description:
  // Sets the device interactor to use
  void SetDeviceInteractor(vtkDeviceInteractor*);

protected:
  vtkWin32RenderWindowDeviceInteractor();
  ~vtkWin32RenderWindowDeviceInteractor();

  vtkDeviceInteractor* DeviceInteractor;

private:
  vtkWin32RenderWindowDeviceInteractor(const vtkWin32RenderWindowDeviceInteractor&);  // Not implemented.
  void operator=(const vtkWin32RenderWindowDeviceInteractor&);  // Not implemented.
};

#endif
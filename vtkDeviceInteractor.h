/*=========================================================================

  Name:        vtkDeviceInteractor.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/
// .NAME vtkDeviceInteractor
// .SECTION Description
// vtkDeviceInteractor is used to add interaction with external devices 
// to a vtkRenderWindowInteractor.  Examples of such devices include
// multi-touch interfaces and various devices supported by the Virtual 
// Reality Peripheral Network (VRPN: 
// http://www.cs.unc.edu/Research/vrpn/).  

// .SECTION see also
// vtkInteractionDeviceManager vtkInteractionDevice 
// vtkDeviceInteractorStyle

#ifndef __vtkDeviceInteractor_h
#define __vtkDeviceInteractor_h

#include "vtkInteractionDeviceConfigure.h"

#include "vtkObject.h"

class vtkDeviceInteractorStyle;
class vtkInteractionDevice;

// Holds vtkstd member variables, which should be hidden
class vtkDeviceInteractorInternals;

class VTK_INTERACTIONDEVICE_EXPORT vtkDeviceInteractor : public vtkObject
{
public:
  static vtkDeviceInteractor* New();
  vtkTypeRevisionMacro(vtkDeviceInteractor,vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Updates devices
  void Update();

  // Description:
  // Add/Remove interaction devices
  void AddInteractionDevice(vtkInteractionDevice*);
  void RemoveInteractionDevice(vtkInteractionDevice*);

  // Description:
  // Add/Remove device interactor styles.  This is just used for book-keeping.
  void AddDeviceInteractorStyle(vtkDeviceInteractorStyle*);
  void RemoveDeviceInteractorStyle(vtkDeviceInteractorStyle*);

protected:
  vtkDeviceInteractor();
  ~vtkDeviceInteractor();

  vtkDeviceInteractorInternals* Internals;

private:
  vtkDeviceInteractor(const vtkDeviceInteractor&);  // Not implemented.
  void operator=(const vtkDeviceInteractor&);  // Not implemented.
};

#endif
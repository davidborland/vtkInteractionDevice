/*=========================================================================

  Name:        vtkInteractionDeviceManager.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/
// .NAME vtkInteractionDeviceManager
// .SECTION Description
// vtkInteractionDeviceManager is a class that handles creation of 
// platform-specific subclasses of vtkRenderWindowInteractor that can 
// interface with external devices for interaction.  Examples include
// multi-touch interfaces and various devices supported by the 
// Virtual Reality Peripheral Network (VRPN: 
// http://www.cs.unc.edu/Research/vrpn/).  

// .SECTION see also
// vtkInteractionDevice vtkDeviceInteractorStyle
// vtkRenderWindowInteractor

#ifndef __vtkInteractionDeviceManager_h
#define __vtkInteractionDeviceManager_h

#include "vtkInteractionDeviceConfigure.h"

#include "vtkObject.h"

class vtkDeviceInteractor;
class vtkRenderWindowInteractor;

class VTK_INTERACTIONDEVICE_EXPORT vtkInteractionDeviceManager : public vtkObject
{
public:
  static vtkInteractionDeviceManager* New();
  vtkTypeRevisionMacro(vtkInteractionDeviceManager,vtkObject);
  void PrintSelf(ostream&, vtkIndent);

  // Description:
  // Returns a platform-specific subclass of vtkRenderWindowInteractor
  // that uses a vtkDeviceInteractor to communicates with an external 
  // device for interaction.
  vtkRenderWindowInteractor* GetInteractor(vtkDeviceInteractor* deviceInteractor);

protected:
  vtkInteractionDeviceManager();
  ~vtkInteractionDeviceManager();

private:
  vtkInteractionDeviceManager(const vtkInteractionDeviceManager&);  // Not implemented.
  void operator=(const vtkInteractionDeviceManager&);  // Not implemented.
};

#endif
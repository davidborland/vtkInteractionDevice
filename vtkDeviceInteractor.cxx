/*=========================================================================

  Name:        vtkDeviceInteractor.cxx

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/

#include "vtkDeviceInteractor.h"

#include "vtkCommand.h"
#include "vtkDeviceInteractorStyle.h"
#include "vtkInteractionDevice.h"
#include "vtkObjectFactory.h"
#include "vtkstd/vector"

class vtkDeviceInteractorInternals
{
public:
  vtkstd::vector<vtkInteractionDevice*> InteractionDevices;
  vtkstd::vector<vtkDeviceInteractorStyle*> DeviceInteractorStyles;
};

vtkCxxRevisionMacro(vtkDeviceInteractor, "$Revision: 1.0 $");
vtkStandardNewMacro(vtkDeviceInteractor);

//----------------------------------------------------------------------------
vtkDeviceInteractor::vtkDeviceInteractor() 
{
  this->Internals = new vtkDeviceInteractorInternals;
}

//----------------------------------------------------------------------------
vtkDeviceInteractor::~vtkDeviceInteractor()
{
  for (unsigned int i = 0; i < this->Internals->InteractionDevices.size(); i++)
    {
    this->Internals->InteractionDevices[i]->UnRegister(this);
    }

  for (unsigned int i = 0; i < this->Internals->DeviceInteractorStyles.size(); i++)
    {
    this->Internals->DeviceInteractorStyles[i]->UnRegister(this);
    }

  delete this->Internals;
}

//----------------------------------------------------------------------------
void vtkDeviceInteractor::Update()
{
  for (unsigned int i = 0; i < this->Internals->InteractionDevices.size(); i++) 
    {
    this->Internals->InteractionDevices[i]->Update();
    this->Internals->InteractionDevices[i]->InvokeInteractionEvent();
    }
}

//----------------------------------------------------------------------------
void vtkDeviceInteractor::AddInteractionDevice(vtkInteractionDevice* device)
{
  if (device == NULL) return;

  for (unsigned int i = 0; i < this->Internals->InteractionDevices.size(); i++) 
    {
    if (this->Internals->InteractionDevices[i] == device) return;
    }

  this->Internals->InteractionDevices.push_back(device);

  this->Internals->InteractionDevices.back()->Register(this);
}

//----------------------------------------------------------------------------
void vtkDeviceInteractor::RemoveInteractionDevice(vtkInteractionDevice* device)
{
  if (device == NULL) return;

  for (unsigned int i = 0; i < this->Internals->InteractionDevices.size(); i++) 
    {
    if (this->Internals->InteractionDevices[i] == device) 
      {
      this->Internals->InteractionDevices[i]->UnRegister(this);
      this->Internals->InteractionDevices.erase(this->Internals->InteractionDevices.begin() + i);

      return;
      }
    }
}

//----------------------------------------------------------------------------
void vtkDeviceInteractor::AddDeviceInteractorStyle(vtkDeviceInteractorStyle* device)
{
  if (device == NULL) return;

  for (unsigned int i = 0; i < this->Internals->DeviceInteractorStyles.size(); i++) 
    {
    if (this->Internals->DeviceInteractorStyles[i] == device) return;
    }

  this->Internals->DeviceInteractorStyles.push_back(device);

  this->Internals->DeviceInteractorStyles.back()->Register(this);
}

//----------------------------------------------------------------------------
void vtkDeviceInteractor::RemoveDeviceInteractorStyle(vtkDeviceInteractorStyle* device)
{
  if (device == NULL) return;

  for (unsigned int i = 0; i < this->Internals->DeviceInteractorStyles.size(); i++) 
    {
    if (this->Internals->DeviceInteractorStyles[i] == device) 
      {
      this->Internals->DeviceInteractorStyles[i]->UnRegister(this);
      this->Internals->DeviceInteractorStyles.erase(this->Internals->DeviceInteractorStyles.begin() + i);

      return;
      }
    }
}

//----------------------------------------------------------------------------
void vtkDeviceInteractor::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "InteractionDevices:" << endl;
  for (unsigned int i = 0; i < this->Internals->InteractionDevices.size(); i++)
    {
    os << indent; this->Internals->InteractionDevices[i]->PrintSelf(os,indent.GetNextIndent());
    }
  os << indent << "DeviceInteractorStyles:" << endl;
  for (unsigned int i = 0; i < this->Internals->DeviceInteractorStyles.size(); i++)
    {
    os << indent; this->Internals->DeviceInteractorStyles[i]->PrintSelf(os,indent.GetNextIndent());
    }
}
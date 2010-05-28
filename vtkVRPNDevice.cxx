/*=========================================================================

  Name:        vtkVRPNDevice.cxx

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/

#include "vtkVRPNDevice.h"

vtkCxxRevisionMacro(vtkVRPNDevice, "$Revision: 1.0 $");

//----------------------------------------------------------------------------
vtkVRPNDevice::vtkVRPNDevice() 
{
  this->DeviceName = NULL;
}

//----------------------------------------------------------------------------
vtkVRPNDevice::~vtkVRPNDevice() 
{  
  if (this->DeviceName)
    {
    delete [] this->DeviceName;
    }
}

//----------------------------------------------------------------------------
void vtkVRPNDevice::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "DeviceName: " << this->DeviceName << "\n";
}

/*=========================================================================

  Name:        vtkInteractionDevice.cxx

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/

#include "vtkInteractionDevice.h"

vtkCxxRevisionMacro(vtkInteractionDevice, "$Revision: 1.0 $");

//----------------------------------------------------------------------------
vtkInteractionDevice::vtkInteractionDevice() 
{
}

//----------------------------------------------------------------------------
vtkInteractionDevice::~vtkInteractionDevice() 
{
}

//----------------------------------------------------------------------------
void vtkInteractionDevice::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

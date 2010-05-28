/*=========================================================================

  Name:        vtkWiiMoteStyle.cxx

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/

#include "vtkWiiMoteStyle.h"

#include "vtkCallbackCommand.h"
#include "vtkVRPNAnalog.h"
#include "vtkVRPNAnalogOutput.h"
#include "vtkVRPNButton.h"

vtkCxxRevisionMacro(vtkWiiMoteStyle, "$Revision: 1.0 $");

vtkCxxSetObjectMacro(vtkWiiMoteStyle, AnalogOutput, vtkVRPNAnalogOutput);

//----------------------------------------------------------------------------
vtkWiiMoteStyle::vtkWiiMoteStyle() 
{ 
  this->AnalogOutput = NULL;
}

//----------------------------------------------------------------------------
vtkWiiMoteStyle::~vtkWiiMoteStyle() 
{
  this->SetAnalogOutput(NULL);
}

//----------------------------------------------------------------------------
void vtkWiiMoteStyle::SetAnalog(vtkVRPNAnalog* analog)
{  
  if (analog != NULL) 
    {
    analog->SetNumberOfChannels(16);
    analog->AddObserver(vtkVRPNDevice::AnalogEvent, this->DeviceCallback);
    }
} 

//----------------------------------------------------------------------------
void vtkWiiMoteStyle::SetButton(vtkVRPNButton* button)
{  
  if (button != NULL) 
    {
    button->SetNumberOfButtons(16);
    button->AddObserver(vtkVRPNDevice::ButtonEvent, this->DeviceCallback);
    }
} 

//----------------------------------------------------------------------------
void vtkWiiMoteStyle::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "AnalogOutput: " << this->AnalogOutput << "\n";
}
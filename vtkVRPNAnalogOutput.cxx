/*=========================================================================

  Name:        vtkVRPNAnalogOutput.cxx

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/

#include "vtkVRPNAnalogOutput.h"

#include "vtkObjectFactory.h"
#include "vtkstd/vector"

#include <vrpn_Analog_Output.h>

vtkCxxRevisionMacro(vtkVRPNAnalogOutput, "$Revision: 1.0 $");
vtkStandardNewMacro(vtkVRPNAnalogOutput);

//----------------------------------------------------------------------------
vtkVRPNAnalogOutput::vtkVRPNAnalogOutput() 
{
  this->AnalogOutput = NULL;
}

//----------------------------------------------------------------------------
vtkVRPNAnalogOutput::~vtkVRPNAnalogOutput() 
{
  if (this->AnalogOutput) delete this->AnalogOutput;
}

//----------------------------------------------------------------------------
int vtkVRPNAnalogOutput::Initialize() 
{
  // Check that the device name is set
  if (this->DeviceName == NULL) 
    {
    vtkErrorMacro(<<"DeviceName not set.");
    return 0;
    }

  // Create the VRPN analog remote 
  this->AnalogOutput = new vrpn_Analog_Output_Remote(this->DeviceName);

  return 1;
}

//----------------------------------------------------------------------------
void vtkVRPNAnalogOutput::Update() 
{
  if (this->AnalogOutput)
    {
    this->AnalogOutput->mainloop();
    }
}

//----------------------------------------------------------------------------
void vtkVRPNAnalogOutput::SetChannel(int channel, double value)
{
  this->AnalogOutput->request_change_channel_value(channel, value);
}

//----------------------------------------------------------------------------
void vtkVRPNAnalogOutput::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "AnalogOutput: " << this->AnalogOutput << "\n";
}

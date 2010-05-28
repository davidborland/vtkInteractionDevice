/*=========================================================================

  Name:        vtkVRPNAnalog.cxx

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/

#include "vtkVRPNAnalog.h"

#include "vtkObjectFactory.h"
#include "vtkstd/vector"

#include <vrpn_Analog.h>

class vtkVRPNAnalogInternals 
{
public:
  vtkstd::vector<double> Channel;
};

vtkCxxRevisionMacro(vtkVRPNAnalog, "$Revision: 1.0 $");
vtkStandardNewMacro(vtkVRPNAnalog);

// Callbacks
static void VRPN_CALLBACK HandleAnalog(void* userData, const vrpn_ANALOGCB a);

//----------------------------------------------------------------------------
vtkVRPNAnalog::vtkVRPNAnalog() 
{
  this->Internals = new vtkVRPNAnalogInternals();

  this->Analog = NULL;

  this->SetNumberOfChannels(1);
}

//----------------------------------------------------------------------------
vtkVRPNAnalog::~vtkVRPNAnalog() 
{
  if (this->Analog) delete this->Analog;

  delete this->Internals;
}

//----------------------------------------------------------------------------
int vtkVRPNAnalog::Initialize() 
{
  // Check that the device name is set
  if (this->DeviceName == NULL) 
    {
    vtkErrorMacro(<<"DeviceName not set.");
    return 0;
    }

  // Create the VRPN analog remote 
  this->Analog = new vrpn_Analog_Remote(this->DeviceName);

  // Set up the analog callback
  if (this->Analog->register_change_handler(this, HandleAnalog) == -1)
    {
    vtkErrorMacro(<<"Can't register callback.");
    return 0;
    }

  return 1;
}

//----------------------------------------------------------------------------
void vtkVRPNAnalog::Update() 
{
  if (this->Analog)
    {
    this->Analog->mainloop();
    }
}

//----------------------------------------------------------------------------
void vtkVRPNAnalog::InvokeInteractionEvent() 
{
  if (this->Analog)
    {
    // XXX: Should there be a flag to check for new data?
    this->InvokeEvent(vtkVRPNDevice::AnalogEvent);
    }
}

//----------------------------------------------------------------------------
void vtkVRPNAnalog::SetNumberOfChannels(int num) 
{
  int currentNum = this->Internals->Channel.size();

  this->Internals->Channel.resize(num);

  for (int i = currentNum; i < num; i++) 
    {
    this->SetChannel(i, 0.0);
    }
}

//----------------------------------------------------------------------------
int vtkVRPNAnalog::GetNumberOfChannels() 
{
  return this->Internals->Channel.size();
}

//----------------------------------------------------------------------------
void vtkVRPNAnalog::SetChannel(int channel, double value)
{
  this->Internals->Channel[channel] = value;
}

//----------------------------------------------------------------------------
double vtkVRPNAnalog::GetChannel(int channel)
{
  return this->Internals->Channel[channel];
}

//----------------------------------------------------------------------------
void VRPN_CALLBACK HandleAnalog(void* userData, const vrpn_ANALOGCB a) {
  vtkVRPNAnalog* analog = static_cast<vtkVRPNAnalog*>(userData);

  int num = analog->GetNumberOfChannels() < a.num_channel ? analog->GetNumberOfChannels() : a.num_channel;

  for (int i = 0; i < num; i++)
    {
    analog->SetChannel(i, a.channel[i]);
    }
}

//----------------------------------------------------------------------------
void vtkVRPNAnalog::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "Analog: "; this->Analog->print();
  os << indent << "Channel: ";
  for (unsigned int i = 0; i < this->Internals->Channel.size(); i++) 
    {
    os << this->Internals->Channel[i] << " ";
    }
  os << "\n";
}

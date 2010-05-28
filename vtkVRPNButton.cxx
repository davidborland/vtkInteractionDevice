/*=========================================================================

  Name:        vtkVRPNButton.cxx

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/

#include "vtkVRPNButton.h"

#include "vtkObjectFactory.h"
#include "vtkstd/vector"

#include <vrpn_Button.h>

class vtkVRPNButtonInternals
{
public:
  vtkstd::vector<bool> Buttons;
};

// Callbacks
static void VRPN_CALLBACK HandleButton(void* userData, const vrpn_BUTTONCB b);

vtkCxxRevisionMacro(vtkVRPNButton, "$Revision: 1.0 $");
vtkStandardNewMacro(vtkVRPNButton);

//----------------------------------------------------------------------------
vtkVRPNButton::vtkVRPNButton() 
{
  this->Internals = new vtkVRPNButtonInternals;

  this->Button = NULL;

  this->SetNumberOfButtons(1);
}

//----------------------------------------------------------------------------
vtkVRPNButton::~vtkVRPNButton() 
{
  if (this->Button) delete this->Button;

  delete this->Internals;
}

//----------------------------------------------------------------------------
int vtkVRPNButton::Initialize() 
{
  // Check that the device name is set
  if (this->DeviceName == NULL) 
    {
    vtkErrorMacro(<<"DeviceName not set.");
    return 0;
    }

  // Create the VRPN Button remote 
  this->Button = new vrpn_Button_Remote(this->DeviceName);

  // Set up the Button callback
  if (this->Button->register_change_handler(this, HandleButton) == -1)
    {
    vtkErrorMacro(<<"Can't register callback.");
    return 0;
    }

  return 1;
}

//----------------------------------------------------------------------------
void vtkVRPNButton::Update() 
{
  if (this->Button)
    {
    this->Button->mainloop();
    }
}

//----------------------------------------------------------------------------
void vtkVRPNButton::InvokeInteractionEvent() 
{
  if (this->Button)
    {
    // XXX: Should there be a flag to check for new data?
    this->InvokeEvent(vtkVRPNDevice::ButtonEvent);
    }
}

//----------------------------------------------------------------------------
void vtkVRPNButton::SetNumberOfButtons(int num) 
{
  int currentNum = this->Internals->Buttons.size();

  this->Internals->Buttons.resize(num);

  for (int i = currentNum; i < num; i++) 
    {
    this->SetButton(i, false);
    }
}

//----------------------------------------------------------------------------
int vtkVRPNButton::GetNumberOfButtons() 
{
  return this->Internals->Buttons.size();
}

//----------------------------------------------------------------------------
void vtkVRPNButton::SetButton(int button, bool value)
{
  this->Internals->Buttons[button] = value;
}

//----------------------------------------------------------------------------
bool vtkVRPNButton::GetButton(int button)
{
  return this->Internals->Buttons[button];
}

//----------------------------------------------------------------------------
void vtkVRPNButton::SetToggle(int button, bool toggle) 
{
  if (!this->Button) return;

  if (toggle) this->Button->set_toggle(button, vrpn_BUTTON_TOGGLE_ON);
  else this->Button->set_momentary(button);
}

//----------------------------------------------------------------------------
void VRPN_CALLBACK HandleButton(void* userData, const vrpn_BUTTONCB b) {
  vtkVRPNButton* button = static_cast<vtkVRPNButton*>(userData);

  if (b.button < button->GetNumberOfButtons())
    {
    button->SetButton(b.button, b.state != 0);
    }
}

//----------------------------------------------------------------------------
void vtkVRPNButton::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "Button: "; Button->print();

  os << indent << "Buttons: ";
  for (unsigned int i = 0; i < this->Internals->Buttons.size(); i++) 
    {
    os << this->Internals->Buttons[i] << " ";
    }
  os << "\n";
}

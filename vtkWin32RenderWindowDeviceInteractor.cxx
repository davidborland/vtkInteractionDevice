/*=========================================================================

  Name:        vtkWin32RenderWindowDeviceInteractor.cxx

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/

#include "vtkWin32RenderWindowDeviceInteractor.h"

#include "vtkCommand.h"
#include "vtkDeviceInteractor.h"
#include "vtkObjectFactory.h"

#ifndef VTK_IMPLEMENT_MESA_CXX
vtkCxxRevisionMacro(vtkWin32RenderWindowDeviceInteractor, "$Revision: 1.0 $");
vtkStandardNewMacro(vtkWin32RenderWindowDeviceInteractor);
#endif

vtkCxxSetObjectMacro(vtkWin32RenderWindowDeviceInteractor, DeviceInteractor, vtkDeviceInteractor);

//----------------------------------------------------------------------------
vtkWin32RenderWindowDeviceInteractor::vtkWin32RenderWindowDeviceInteractor() 
{
  this->DeviceInteractor = NULL;
}

//----------------------------------------------------------------------
vtkWin32RenderWindowDeviceInteractor::~vtkWin32RenderWindowDeviceInteractor()
{
  this->SetDeviceInteractor(NULL);
}

//----------------------------------------------------------------------
void vtkWin32RenderWindowDeviceInteractor::Start() 
{
  // Let the compositing handle the event loop if it wants to.
  if (this->HasObserver(vtkCommand::StartEvent) && !this->HandleEventLoop)
    {
    this->InvokeEvent(vtkCommand::StartEvent,NULL);
    return;
    }

  // No need to do anything if this is a 'mapped' interactor
  if (!this->Enabled || !this->InstallMessageProc)
    {
    return;
    }

  this->StartedMessageLoop = 1;

  MSG msg;
  while (1)
    { 
    // Need to use PeekMessage() instead of looping on GetMessage() so 
    // that the InteractionDevice code gets called.
    PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
    TranslateMessage(&msg);
    DispatchMessage(&msg);

    // Receive updates from interaction devices
    if (this->DeviceInteractor) 
      {
      this->DeviceInteractor->Update();
      this->Render();
      }
    }
}

//----------------------------------------------------------------------------
void vtkWin32RenderWindowDeviceInteractor::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "DeviceInteractor: ";
  this->DeviceInteractor->PrintSelf(os,indent.GetNextIndent());
}
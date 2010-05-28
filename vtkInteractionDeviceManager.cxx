/*=========================================================================

  Name:        vtkInteractionDeviceManager.cxx

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/

#include "vtkInteractionDeviceManager.h"

#include "vtkDeviceInteractor.h"
#include "vtkGraphicsFactory.h"
#include "vtkObjectFactory.h"
#include "vtkRenderWindowInteractor.h"

// Include platform-specific headers via code borrowed from vtkGraphicsFactory.h

// Win32 specific stuff
#ifdef _WIN32
# ifndef VTK_USE_OGLR
#  include "vtkWin32RenderWindowDeviceInteractor.h"
#  define VTK_DISPLAY_WIN32_OGL
# endif // VTK_USE_OGLR
#endif

// Apple OSX stuff
#ifdef VTK_USE_CARBON
# include "vtkCarbonRenderWindowDeviceInteractor.h"
# define VTK_DISPLAY_CARBON
#endif

#ifdef VTK_USE_COCOA
# include "vtkCocoaRenderWindowDeviceInteractor.h"
# define VTK_DISPLAY_COCOA
#endif

// X OpenGL stuff
#ifdef VTK_USE_OGLR
# include "vtkXOpenGLRenderWindowDeviceInteractor.h"
# define VTK_DISPLAY_X11_OGL
#endif

// OSMESA OpenGL stuff
#ifdef VTK_USE_OSMESA
# include "vtkOSOpenGLRenderWindowDeviceInteractor.h"
//# define VTK_DISPLAY_X11_OGL
#endif

#if defined(VTK_USE_MANGLED_MESA)
#include "vtkXMesaRenderWindowDeviceInteractor.h"
#endif

vtkCxxRevisionMacro(vtkInteractionDeviceManager, "$Revision: 1.0 $");
vtkStandardNewMacro(vtkInteractionDeviceManager);

//----------------------------------------------------------------------------
vtkInteractionDeviceManager::vtkInteractionDeviceManager() 
{
}

//----------------------------------------------------------------------------
vtkInteractionDeviceManager::~vtkInteractionDeviceManager() 
{
}

//----------------------------------------------------------------------------
vtkRenderWindowInteractor* vtkInteractionDeviceManager::GetInteractor(vtkDeviceInteractor* deviceInteractor)
{
  // Return the correct platform-specific vtkRenderWindowInteractor subclass

  const char *rl = vtkGraphicsFactory::GetRenderLibrary();

#ifdef VTK_USE_OGLR
  if (!vtkGraphicsFactory::GetOffScreenOnlyMode())
    {
    vtkErrorMacro(<<"Device interaction not implemented for this platform yet.");
    return vtkXRenderWindowInteractor::New();
    }
#endif

#if defined(VTK_USE_OSMESA)
  vtkErrorMacro(<<"Device interaction not implemented for this platform yet.");
  return vtkRenderWindowInteractor::New();
#endif

#ifdef VTK_DISPLAY_WIN32_OGL
  if (!strcmp("Win32OpenGL",rl))
    {
    vtkWin32RenderWindowDeviceInteractor* interactor = vtkWin32RenderWindowDeviceInteractor::New();
    interactor->SetDeviceInteractor(deviceInteractor);

    return interactor;
    }
#endif

#ifdef VTK_USE_CARBON
  if (!vtkGraphicsFactory::GetOffScreenOnlyMode())
    {
    vtkErrorMacro(<<"Device interaction not implemented for this platform yet.");
    return vtkCarbonRenderWindowInteractor::New();
    }
#endif
#ifdef VTK_USE_COCOA
  if (!vtkGraphicsFactory::GetOffScreenOnlyMode())
    {
    vtkErrorMacro(<<"Device interaction not implemented for this platform yet.");
    return vtkCocoaRenderWindowInteractor::New();
    }
#endif

  return 0;
}

//----------------------------------------------------------------------------
void vtkInteractionDeviceManager::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
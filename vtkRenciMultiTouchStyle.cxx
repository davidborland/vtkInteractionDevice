/*=========================================================================

  Name:        vtkRenciMultiTouchStyle.cxx

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/

#include "vtkRenciMultiTouchStyle.h"

#include "vtkCallbackCommand.h"

#include "vtkRenciMultiTouch.h"

vtkCxxRevisionMacro(vtkRenciMultiTouchStyle, "$Revision: 1.0 $");

//----------------------------------------------------------------------------
vtkRenciMultiTouchStyle::vtkRenciMultiTouchStyle() 
{ 
}

//----------------------------------------------------------------------------
vtkRenciMultiTouchStyle::~vtkRenciMultiTouchStyle() 
{
}

//----------------------------------------------------------------------------
void vtkRenciMultiTouchStyle::SetMultiTouch(vtkRenciMultiTouch* multiTouch)
{  
  if (multiTouch != NULL) 
    {
    // Touch and drag events
    multiTouch->AddObserver(vtkRenciMultiTouch::OneTouchEvent, this->DeviceCallback);
    multiTouch->AddObserver(vtkRenciMultiTouch::OneDragEvent, this->DeviceCallback);
    multiTouch->AddObserver(vtkRenciMultiTouch::TwoTouchEvent, this->DeviceCallback);
    multiTouch->AddObserver(vtkRenciMultiTouch::TwoDragEvent, this->DeviceCallback);
    multiTouch->AddObserver(vtkRenciMultiTouch::ThreeTouchEvent, this->DeviceCallback);
    multiTouch->AddObserver(vtkRenciMultiTouch::ThreeDragEvent, this->DeviceCallback);
    multiTouch->AddObserver(vtkRenciMultiTouch::FourTouchEvent, this->DeviceCallback);
    multiTouch->AddObserver(vtkRenciMultiTouch::FourDragEvent, this->DeviceCallback);
    multiTouch->AddObserver(vtkRenciMultiTouch::FiveTouchEvent, this->DeviceCallback);
    multiTouch->AddObserver(vtkRenciMultiTouch::FiveDragEvent, this->DeviceCallback);
    multiTouch->AddObserver(vtkRenciMultiTouch::SixTouchEvent, this->DeviceCallback);
    multiTouch->AddObserver(vtkRenciMultiTouch::SixDragEvent, this->DeviceCallback);

    // Zoom event
    multiTouch->AddObserver(vtkRenciMultiTouch::ZoomEvent, this->DeviceCallback);

    // Translate events
    multiTouch->AddObserver(vtkRenciMultiTouch::TranslateYEvent, this->DeviceCallback);
    multiTouch->AddObserver(vtkRenciMultiTouch::TranslateYEvent, this->DeviceCallback);
    multiTouch->AddObserver(vtkRenciMultiTouch::TranslateZEvent, this->DeviceCallback);

    // Rotate events
    multiTouch->AddObserver(vtkRenciMultiTouch::RotateXEvent, this->DeviceCallback);
    multiTouch->AddObserver(vtkRenciMultiTouch::RotateYEvent, this->DeviceCallback);
    multiTouch->AddObserver(vtkRenciMultiTouch::RotateZEvent, this->DeviceCallback);

    // Release event
    multiTouch->AddObserver(vtkRenciMultiTouch::ReleaseEvent, this->DeviceCallback);
    }
}

//----------------------------------------------------------------------------
void vtkRenciMultiTouchStyle::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
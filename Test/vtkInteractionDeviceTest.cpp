/*=========================================================================

  Name:        vtkInteractionDeviceTest.cpp

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included RENCI_License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

  Description: Program for testing vtkInteractionDevice library.

=========================================================================*/


#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkDeviceInteractor.h>
#include <vtkInteractionDeviceManager.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenciMultiTouch.h>
#include <vtkRenciMultiTouchStyleCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVRPNAnalog.h>
#include <vtkVRPNAnalogOutput.h>
#include <vtkVRPNButton.h>
#include <vtkVRPNTracker.h>
#include <vtkVRPNTrackerStyleCamera.h>
#include <vtkWiiMoteStyleCamera.h>


// 0 for VRPN tracker, 1 for Renci multi-touch, 2 for VRPN wiimote
int mode = 0;


void main(int argc, char* argv[]) {
    // Normal geometry creation
    vtkConeSource* cone = vtkConeSource::New();
    
    vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
    mapper->SetInputConnection(cone->GetOutputPort());

    vtkActor* actor = vtkActor::New();
    actor->SetMapper(mapper);


    // Set up rendering
    vtkRenderer* renderer = vtkRenderer::New();
    renderer->AddViewProp(actor);

    vtkRenderWindow* window = vtkRenderWindow::New();
    window->AddRenderer(renderer);

    // Set up interaction
    vtkInteractorStyleTrackballCamera* interactorStyle = vtkInteractorStyleTrackballCamera::New();


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Begin vtkInteractionDevice stuff


    vtkInteractionDevice* device1 = NULL;
    vtkInteractionDevice* device2 = NULL;
    vtkVRPNAnalogOutput* analogOutput = NULL;
    vtkDeviceInteractorStyle* deviceStyle = NULL;
    if (mode == 0) {
        // A vtkInteractionDevice, such as a vtkVRPNTracker, receives updates from an external device
        device1 = vtkVRPNTracker::New();
        vtkVRPNTracker* tracker = (vtkVRPNTracker*)device1;
        tracker->SetDeviceName("tracker@localhost");
        tracker->Initialize();

        // A vtkDeviceInteractorStyle, such as vtkVRPNTrackerStyleCamera, listens for events from 
        // vtkInteractionDevices and performs interactions based on these events.
        deviceStyle = vtkVRPNTrackerStyleCamera::New();
        vtkVRPNTrackerStyleCamera* trackerStyleCamera = (vtkVRPNTrackerStyleCamera*)deviceStyle;
        trackerStyleCamera->SetTracker(tracker);
        trackerStyleCamera->SetRenderer(renderer);
    }
    else if (mode == 1) {
        // A vtkInteractionDevice, such as a vtkRenciMultiTouch, receives updates from an external device
        device1 = vtkRenciMultiTouch::New();
        vtkRenciMultiTouch* multiTouch = (vtkRenciMultiTouch*)device1;
        multiTouch->SetHostName("127.0.0.1");
        multiTouch->SetPort(50003);
        multiTouch->Initialize();

        // A vtkDeviceInteractorStyle, such as vtkRenciMultiTouchStyleCamera, listens for events from 
        // vtkInteractionDevices and performs interactions based on these events.                
        deviceStyle = vtkRenciMultiTouchStyleCamera::New();
        vtkRenciMultiTouchStyleCamera* multiTouchStyleCamera = (vtkRenciMultiTouchStyleCamera*)deviceStyle;
        multiTouchStyleCamera->SetMultiTouch(multiTouch);
        multiTouchStyleCamera->SetRenderer(renderer);
    }
    else if (mode == 2) {
        // A vtkInteractionDevice, such as a vtkVRPNAnalog, communicates with an external device
        device1 = vtkVRPNAnalog::New();
        vtkVRPNAnalog* analog = (vtkVRPNAnalog*)device1;
        analog->SetDeviceName("wiimote@localhost");
        analog->Initialize();

        // A vtkInteractionDevice, such as a vtkVRPNAnalogOutput, communicates with an external device
        analogOutput = vtkVRPNAnalogOutput::New();
        analogOutput->SetDeviceName("wiimote@localhost");
        analogOutput->Initialize();

        // A vtkInteractionDevice, such as a vtkVRPNButton, communicates with an external device
        device2 = vtkVRPNButton::New();
        vtkVRPNButton* button = (vtkVRPNButton*)device2;
        button->SetDeviceName("wiimote@localhost");
        button->Initialize();

        // A vtkDeviceInteractorStyle, such as vtkVRPNWiiMoteStyleCamera, listens for events from 
        // vtkInteractionDevices and performs interactions based on these events.
        deviceStyle = vtkWiiMoteStyleCamera::New();
        vtkWiiMoteStyleCamera* wiiMoteStyleCamera = (vtkWiiMoteStyleCamera*)deviceStyle;
        wiiMoteStyleCamera->SetAnalog(analog);
        wiiMoteStyleCamera->SetAnalogOutput(analogOutput);
        wiiMoteStyleCamera->SetButton(button);
        wiiMoteStyleCamera->SetRenderer(renderer);
    }

    // A vtkDeviceInteractor is a container for vtkInteractionDevices and
    // vtkDeviceInteractorStyles that is used by the platform-specific subclasses
    // of vtkRenderWindowInteractor returned by vtkInteractionDeviceManager.
    vtkDeviceInteractor* deviceInteractor = vtkDeviceInteractor::New();
    deviceInteractor->AddInteractionDevice(device1);
    deviceInteractor->AddInteractionDevice(device2);
    deviceInteractor->AddInteractionDevice(analogOutput);
    deviceInteractor->AddDeviceInteractorStyle(deviceStyle);

    // A vtkInteractionDeviceManager returns a platform-specific subclass of 
    // vtkRenderWindowInteractor that interfaces with external devices via
    // a vtkDeviceInteractor
    vtkInteractionDeviceManager* manager = vtkInteractionDeviceManager::New();

    vtkRenderWindowInteractor* interactor = manager->GetInteractor(deviceInteractor);


// End vtkInteractionDevice stuff
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    interactor->SetRenderWindow(window);
    interactor->SetInteractorStyle(interactorStyle);

    interactor->Initialize();


    // Clean up
    cone->Delete();
    mapper->Delete();
    actor->Delete();
    renderer->Delete();
    window->Delete();
    interactorStyle->Delete();
    device1->Delete();
    if (device2) device2->Delete();
    if (analogOutput) analogOutput->Delete();
    deviceStyle->Delete();
    deviceInteractor->Delete();
    manager->Delete();


    // Start interacting
    interactor->Start();
}
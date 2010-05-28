/*=========================================================================

  Name:        vtkVRPNTracker.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/
// .NAME vtkVRPNTracker
// .SECTION Description
// vtkVRPNTracker interfaces with a tracking device using the Virtual 
// Reality Peripheral Network (VRPN: 
// http://www.cs.unc.edu/Research/vrpn/).  

// .SECTION see also
// vtkInteractionDeviceManager vtkDeviceInteractorStyle

#ifndef __vtkVRPNTracker_h
#define __vtkVRPNTracker_h

#include "vtkInteractionDeviceConfigure.h"

#include "vtkVRPNDevice.h"

class vrpn_Tracker_Remote;

// Holds vtkstd member variables, which must be hidden
class vtkVRPNTrackerInternals;

class VTK_INTERACTIONDEVICE_EXPORT vtkVRPNTracker : public vtkVRPNDevice
{
public:
  static vtkVRPNTracker* New();
  vtkTypeRevisionMacro(vtkVRPNTracker,vtkVRPNDevice);
  void PrintSelf(ostream&, vtkIndent);

  // Description:
  // Initialize the device
  virtual int Initialize();

  // Description:
  // Receive updates from the device
  virtual void Update();

  // Description:
  // Invoke vrpnDevice::TrackerEvent for observers to listen for
  virtual void InvokeInteractionEvent();

  // Description:
  // The number of sensors to use
  virtual void SetNumberOfSensors(int num);
  virtual int GetNumberOfSensors();

  // Description:
  // Set/Get the tracker information.  
  // Can't use built-in VTK macros, as we need to index the sensor.
  // Can't use our own macros, as we'd need to access the Internals
  // from a header file.
  void SetPosition(double* position, int sensor = 0);
  double *GetPosition(int sensor = 0);
  void SetRotation(double* rotation, int sensor = 0);
  double *GetRotation(int sensor = 0);

  void SetVelocity(double* velocity, int sensor = 0);
  double *GetVelocity(int sensor = 0);
  void SetVelocityRotation(double* rotation, int sensor = 0);
  double *GetVelocityRotation(int sensor = 0);
  void SetVelocityRotationDelta(double delta, int sensor = 0);
  double GetVelocityRotationDelta(int sensor = 0);

  void SetAcceleration(double* acceleration, int sensor = 0);
  double *GetAcceleration(int sensor = 0);
  void SetAccelerationRotation(double* rotation, int sensor = 0);
  double *GetAccelerationRotation(int sensor = 0);
  void SetAccelerationRotationDelta(double delta, int sensor = 0);
  double GetAccelerationRotationDelta(int sensor = 0);

  void SetUnit2SensorTranslation(double* translation, int sensor = 0);
  double *GetUnit2SensorTranslation(int sensor = 0);
  void SetUnit2SensorRotation(double* rotation, int sensor = 0);
  double *GetUnit2SensorRotation(int sensor = 0);

  // Description:
  // Transformation from tracker space to room space
  vtkSetVector3Macro(Tracker2RoomTranslation,double);
  vtkGetVector3Macro(Tracker2RoomTranslation,double);
  vtkSetVector4Macro(Tracker2RoomRotation,double);
  vtkGetVector4Macro(Tracker2RoomRotation,double);

protected:
  vtkVRPNTracker();
  ~vtkVRPNTracker();

  vrpn_Tracker_Remote* Tracker;

  double Tracker2RoomTranslation[3];
  double Tracker2RoomRotation[4];

  vtkVRPNTrackerInternals* Internals;

private:
  vtkVRPNTracker(const vtkVRPNTracker&);  // Not implemented.
  void operator=(const vtkVRPNTracker&);  // Not implemented.
};

#endif

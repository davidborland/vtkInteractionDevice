/*=========================================================================

  Name:        vtkVRPNTracker.cxx

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/

#include "vtkVRPNTracker.h"

#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkstd/vector"

#include <vrpn_Tracker.h>

// Structure to hold tracker information
struct TrackerInformation 
{
  double Position[3];
  double Rotation[4];

  double Velocity[3];
  double VelocityRotation[4];
  double VelocityRotationDelta;

  double Acceleration[3];
  double AccelerationRotation[4];
  double AccelerationRotationDelta;

  // Unit to sensor transformations.  Need one per sensor.
  double Unit2SensorTranslation[3];
  double Unit2SensorRotation[3];
};

class vtkVRPNTrackerInternals
{
public:
  vtkstd::vector<TrackerInformation> Sensors;
};

// Callbacks
static void VRPN_CALLBACK HandlePosition(void* userData, const vrpn_TRACKERCB t);
static void VRPN_CALLBACK HandleVelocity(void* userData, const vrpn_TRACKERVELCB t);
static void VRPN_CALLBACK HandleAcceleration(void* userData, const vrpn_TRACKERACCCB t);

vtkCxxRevisionMacro(vtkVRPNTracker, "$Revision: 1.0 $");
vtkStandardNewMacro(vtkVRPNTracker);

//----------------------------------------------------------------------------
vtkVRPNTracker::vtkVRPNTracker() 
{
  this->Internals = new vtkVRPNTrackerInternals();

  this->Tracker = NULL;

  this->SetTracker2RoomTranslation(0.0, 0.0, 0.0);
  this->SetTracker2RoomRotation(1.0, 0.0, 0.0, 0.0);

  this->SetNumberOfSensors(1);
}

//----------------------------------------------------------------------------
vtkVRPNTracker::~vtkVRPNTracker() 
{
  if (this->Tracker) delete this->Tracker;

  delete this->Internals;
}

//----------------------------------------------------------------------------
int vtkVRPNTracker::Initialize() 
{
  // Check that the device name is set
  if (this->DeviceName == NULL) 
    {
    vtkErrorMacro(<<"DeviceName not set.");
    return 0;
    }

  // Create the VRPN tracker remote 
  this->Tracker = new vrpn_Tracker_Remote(this->DeviceName);

  // Set up the tracker callbacks
  if (this->Tracker->register_change_handler(this, HandlePosition) == -1 ||
      this->Tracker->register_change_handler(this, HandleVelocity) == -1 ||
      this->Tracker->register_change_handler(this, HandleAcceleration) == -1)
    {
    vtkErrorMacro(<<"Can't register callback.");
    return 0;
    }

  return 1;
}

//----------------------------------------------------------------------------
void vtkVRPNTracker::Update() 
{
  if (this->Tracker)
    {
    this->Tracker->mainloop();
    }
}

//----------------------------------------------------------------------------
void vtkVRPNTracker::InvokeInteractionEvent() 
{
  if (this->Tracker)
    {
    // XXX: Should there be a flag to check for new data?
    this->InvokeEvent(vtkVRPNDevice::TrackerEvent);
    }
}

//----------------------------------------------------------------------------
void vtkVRPNTracker::SetNumberOfSensors(int num) 
{
  int currentNum = this->Internals->Sensors.size();

  this->Internals->Sensors.resize(num);

  double identityVector[3] = { 0.0, 0.0, 0.0 };
  double identityQuaternion[4] = { 1.0, 0.0, 0.0, 0.0 };

  for (int i = currentNum; i < num; i++) 
    {
    this->SetPosition(identityVector, i);
    this->SetRotation(identityQuaternion, i);
    
    this->SetVelocity(identityVector, i);
    this->SetVelocityRotation(identityQuaternion, i);
    this->SetVelocityRotationDelta(1.0, i);

    this->SetAcceleration(identityVector, i);
    this->SetAccelerationRotation(identityQuaternion, i);
    this->SetAccelerationRotationDelta(1.0, i);

    this->SetUnit2SensorTranslation(identityVector, i);
    this->SetUnit2SensorRotation(identityQuaternion, i);
    }
}

//----------------------------------------------------------------------------
int vtkVRPNTracker::GetNumberOfSensors() 
{
  return this->Internals->Sensors.size();
}

//----------------------------------------------------------------------------
void vtkVRPNTracker::SetPosition(double* position, int sensor)
{
  for (int i = 0; i < 3; i++)
    {
    this->Internals->Sensors[sensor].Position[i] = position[i];
    }
}

//----------------------------------------------------------------------------
double* vtkVRPNTracker::GetPosition(int sensor)
{
  return this->Internals->Sensors[sensor].Position;
}

//----------------------------------------------------------------------------
void vtkVRPNTracker::SetRotation(double* rotation, int sensor)
{
  for (int i = 0; i < 4; i++)
    {
    this->Internals->Sensors[sensor].Rotation[i] = rotation[i];
    }
}

//----------------------------------------------------------------------------
double* vtkVRPNTracker::GetRotation(int sensor)
{
  return this->Internals->Sensors[sensor].Rotation;
}

//----------------------------------------------------------------------------
void vtkVRPNTracker::SetVelocity(double* velocity, int sensor)
{
  for (int i = 0; i < 3; i++)
    {
    this->Internals->Sensors[sensor].Velocity[i] = velocity[i];
    }
}

//----------------------------------------------------------------------------
double* vtkVRPNTracker::GetVelocity(int sensor)
{
  return this->Internals->Sensors[sensor].Velocity;
}

//----------------------------------------------------------------------------
void vtkVRPNTracker::SetVelocityRotation(double* rotation, int sensor)
{
  for (int i = 0; i < 4; i++)
    {
    this->Internals->Sensors[sensor].VelocityRotation[i] = rotation[i];
    }
}

//----------------------------------------------------------------------------
double* vtkVRPNTracker::GetVelocityRotation(int sensor)
{
  return this->Internals->Sensors[sensor].Velocity;
}

//----------------------------------------------------------------------------
void vtkVRPNTracker::SetVelocityRotationDelta(double delta, int sensor)
{
  this->Internals->Sensors[sensor].VelocityRotationDelta = delta;
}

//----------------------------------------------------------------------------
double vtkVRPNTracker::GetVelocityRotationDelta(int sensor)
{
  return this->Internals->Sensors[sensor].VelocityRotationDelta;
}

//----------------------------------------------------------------------------
void vtkVRPNTracker::SetAcceleration(double* acceleration, int sensor)
{
  for (int i = 0; i < 3; i++)
    {
    this->Internals->Sensors[sensor].Acceleration[i] = acceleration[i];
    }
}

//----------------------------------------------------------------------------
double* vtkVRPNTracker::GetAcceleration(int sensor)
{
  return this->Internals->Sensors[sensor].Acceleration;
}

//----------------------------------------------------------------------------
void vtkVRPNTracker::SetAccelerationRotation(double* rotation, int sensor)
{
  for (int i = 0; i < 4; i++)
    {
    this->Internals->Sensors[sensor].AccelerationRotation[i] = rotation[i];
    }
}

//----------------------------------------------------------------------------
double* vtkVRPNTracker::GetAccelerationRotation(int sensor)
{
  return this->Internals->Sensors[sensor].Acceleration;
}

//----------------------------------------------------------------------------
void vtkVRPNTracker::SetAccelerationRotationDelta(double delta, int sensor)
{
  this->Internals->Sensors[sensor].AccelerationRotationDelta = delta;
}

//----------------------------------------------------------------------------
double vtkVRPNTracker::GetAccelerationRotationDelta(int sensor)
{
  return this->Internals->Sensors[sensor].AccelerationRotationDelta;
}

//----------------------------------------------------------------------------
void vtkVRPNTracker::SetUnit2SensorTranslation(double* translation, int sensor)
{
  for (int i = 0; i < 3; i++)
    {
    this->Internals->Sensors[sensor].Unit2SensorTranslation[i] = translation[i];
    }
}

//----------------------------------------------------------------------------
double* vtkVRPNTracker::GetUnit2SensorTranslation(int sensor)
{
  return this->Internals->Sensors[sensor].Unit2SensorTranslation;
}

//----------------------------------------------------------------------------
void vtkVRPNTracker::SetUnit2SensorRotation(double* rotation, int sensor)
{
  for (int i = 0; i < 4; i++)
    {
    this->Internals->Sensors[sensor].Unit2SensorRotation[i] = rotation[i];
    }
}

//----------------------------------------------------------------------------
double* vtkVRPNTracker::GetUnit2SensorRotation(int sensor)
{
  return this->Internals->Sensors[sensor].Unit2SensorRotation;
}

//----------------------------------------------------------------------------
void VRPN_CALLBACK HandlePosition(void* userData, const vrpn_TRACKERCB t) {
  vtkVRPNTracker* tracker = static_cast<vtkVRPNTracker*>(userData);

  if (t.sensor < tracker->GetNumberOfSensors()) 
    {
    // Transform the position
    double pos[3];
    double t2wTrans[3];
    tracker->GetTracker2RoomTranslation(t2wTrans);
    for (int i = 0; i < 3; i++) {
        pos[i] = t.pos[i] + t2wTrans[i];
    }

    // Set the position for this sensor
    tracker->SetPosition(pos, t.sensor);

    // Convert from vrpn quaternion (x, y, z, w) to vtk quaternion (w, x, y, z)
    double vtkQuat[4];
    vtkQuat[0] = t.quat[3];
    vtkQuat[1] = t.quat[0];
    vtkQuat[2] = t.quat[1];
    vtkQuat[3] = t.quat[2];
    
    // Transform the rotation via matrix operations.  
    // Would be nice if VTK had quaternion operations instead...
    double rot[3][3];
    vtkMath::QuaternionToMatrix3x3(vtkQuat, rot);

    double t2wRot[3][3];
    vtkMath::QuaternionToMatrix3x3(tracker->GetTracker2RoomRotation(), t2wRot);

    vtkMath::Multiply3x3(rot, t2wRot, rot);

    vtkMath::Matrix3x3ToQuaternion(rot, vtkQuat);

    // Set the rotation for this sensor
    tracker->SetRotation(vtkQuat, t.sensor);
    }
}

//----------------------------------------------------------------------------
void VRPN_CALLBACK HandleVelocity(void* userData, const vrpn_TRACKERVELCB t) {
  vtkVRPNTracker* tracker = static_cast<vtkVRPNTracker*>(userData);

  if (t.sensor < tracker->GetNumberOfSensors()) 
    {
    // Set the velocity for this sensor
    tracker->SetVelocity((double*)t.vel, t.sensor);
    
    // Convert from vrpn quaternion (x, y, z, w) to vtk quaternion (w, x, y, z)
    double vtkQuat[4];
    vtkQuat[0] = t.vel_quat[3];
    vtkQuat[1] = t.vel_quat[0];
    vtkQuat[2] = t.vel_quat[1];
    vtkQuat[3] = t.vel_quat[2];
    tracker->SetVelocityRotation(vtkQuat, t.sensor);

    // Set the velocity rotation for this sensor
    tracker->SetVelocityRotation(vtkQuat, t.sensor);
    
    // Set the velocity rotation delta for this sensor
    tracker->SetVelocityRotationDelta(t.vel_quat_dt, t.sensor);
    }
}

//----------------------------------------------------------------------------
void VRPN_CALLBACK HandleAcceleration(void* userData, const vrpn_TRACKERACCCB t) {
  vtkVRPNTracker* tracker = static_cast<vtkVRPNTracker*>(userData);

  if (t.sensor < tracker->GetNumberOfSensors()) 
    {
    // Set the acceleration for this sensor
    tracker->SetAcceleration((double*)t.acc, t.sensor);
    
    // Convert from vrpn quaternion (x, y, z, w) to vtk quaternion (w, x, y, z)
    double vtkQuat[4];
    vtkQuat[0] = t.acc_quat[3];
    vtkQuat[1] = t.acc_quat[0];
    vtkQuat[2] = t.acc_quat[1];
    vtkQuat[3] = t.acc_quat[2];
    tracker->SetAccelerationRotation(vtkQuat, t.sensor);

    // Set the acceleration rotation for this sensor
    tracker->SetAccelerationRotation(vtkQuat, t.sensor);

    // Set the acceleration rotation delta for this sensor
    tracker->SetAccelerationRotationDelta(t.acc_quat_dt, t.sensor);
    }
}

//----------------------------------------------------------------------------
void vtkVRPNTracker::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "Tracker: "; Tracker->print_latest_report();

  os << indent << "Sensors:" << endl;
  for (unsigned int i = 0; i < this->Internals->Sensors.size(); i++)
    {
    os << indent << indent << "Position: (" << this->Internals->Sensors[i].Position[0]
                 << ", " << this->Internals->Sensors[i].Position[1]
                 << ", " << this->Internals->Sensors[i].Position[2] << ")\n";
    os << indent << indent << "Rotation: (" << this->Internals->Sensors[i].Rotation[0]
                 << ", " << this->Internals->Sensors[i].Rotation[1]
                 << ", " << this->Internals->Sensors[i].Rotation[2]
                 << ", " << this->Internals->Sensors[i].Rotation[3] << ")\n";
    os << indent << indent << "Velocity: (" << this->Internals->Sensors[i].Velocity[0]
                 << ", " << this->Internals->Sensors[i].Velocity[1]
                 << ", " << this->Internals->Sensors[i].Velocity[2] << ")\n";    
    os << indent << indent << "Velocity Rotation: (" << this->Internals->Sensors[i].VelocityRotation[0]
                 << ", " << this->Internals->Sensors[i].VelocityRotation[1]
                 << ", " << this->Internals->Sensors[i].VelocityRotation[2] 
                 << ", " << this->Internals->Sensors[i].VelocityRotation[3] << ")\n";   
    os << indent << indent << "VelocityRotationDelta: " << this->Internals->Sensors[i].VelocityRotationDelta << "\n";    
    os << indent << indent << "Acceleration: (" << this->Internals->Sensors[i].Acceleration[0]
                 << ", " << this->Internals->Sensors[i].Acceleration[1]
                 << ", " << this->Internals->Sensors[i].Acceleration[2] << ")\n";    
    os << indent << indent << "Acceleration Rotation: (" << this->Internals->Sensors[i].AccelerationRotation[0]
                 << ", " << this->Internals->Sensors[i].AccelerationRotation[1]
                 << ", " << this->Internals->Sensors[i].AccelerationRotation[2]
                 << ", " << this->Internals->Sensors[i].AccelerationRotation[3] << ")\n";   
    os << indent << indent << "AccelerationRotationDelta: " << this->Internals->Sensors[i].AccelerationRotationDelta << "\n";   
    }
}

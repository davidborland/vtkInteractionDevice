/*=========================================================================

  Name:        vtkRenciMultiTouch.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/
// .NAME vtkRenciMultiTouch
// .SECTION Description
// vtkRenciMultiTouch interfaces with multi-touch devices developed at
// the Renaissance Computing Institute 
// (http://vis.renci.org/multitouch/).  

// .SECTION see also
// vtkInteractionDeviceManager vtkDeviceInteractorStyle

#ifndef __vtkRenciMultiTouch_h
#define __vtkRenciMultiTouch_h

#include "vtkInteractionDeviceConfigure.h"

#include "vtkInteractionDevice.h"

#ifdef WIN32
# include "vtkWindows.h"
# include "winsock.h"
#endif

#include "vtkCommand.h"

// Structure defining a touch point.  Gestures can have multiple touch points.
struct TouchPoint 
{
  int Id;
  double Location[2];
  double Direction[2];
  int MoveLocation;
};

// Holds vtkstd member variables, which must be hidden
class vtkRenciMultiTouchInternals;

class VTK_INTERACTIONDEVICE_EXPORT vtkRenciMultiTouch : public vtkInteractionDevice
{
public:
  static vtkRenciMultiTouch* New();
  vtkTypeRevisionMacro(vtkRenciMultiTouch,vtkInteractionDevice);
  void PrintSelf(ostream&, vtkIndent);

  // Description:
  // Initialize the device
  virtual int Initialize();

  // Description:
  // Receive updates from the device
  virtual void Update();

  // Description:
  // Invoke events for observers to listen for
  virtual void InvokeInteractionEvent();

  // Description:
  // Set socket information.  Must be set before Initialize().
  vtkSetStringMacro(HostName);
  vtkSetMacro(Port,int);

  // Description:
  // Get methods for gesture data
  int GetNumberOfTouchPoints();
  const TouchPoint& GetTouchPoint(int which);

  // Enumeration for multi-touch events
  //BTX
  enum RenciMultiTouchEventIds {
      OneTouchEvent = vtkCommand::UserEvent,
      OneDragEvent,
      TwoTouchEvent,
      TwoDragEvent,
      ThreeTouchEvent,
      ThreeDragEvent,
      FourTouchEvent,
      FourDragEvent,
      FiveTouchEvent,
      FiveDragEvent,
      SixTouchEvent,
      SixDragEvent,
      ZoomEvent,
      TranslateXEvent,
      TranslateYEvent,
      TranslateZEvent,
      RotateXEvent,
      RotateYEvent,
      RotateZEvent,
      ReleaseEvent
  };
  //ETX

protected:
  vtkRenciMultiTouch();
  ~vtkRenciMultiTouch();

  // The socket being read
  char* HostName;
  int Port;
  int SocketDescriptor;

  vtkRenciMultiTouchInternals* Internals;

  // Description:
  // Parse the buffer
  void ParseBuffer(char* buffer, int numBytes);

  // Description:
  // Clear the current gesture
  void ClearGesture();

  // Description:
  // Socket code.  vtkSocket currently uses TCP, so leave this code in here for now.
  int CreateSocket();
  int Receive(void* data, int length);
  int ReadInt(char** buffer);
  double ReadDouble(char** buffer);
  double Ntohd(double d);

private:
  vtkRenciMultiTouch(const vtkRenciMultiTouch&);  // Not implemented.
  void operator=(const vtkRenciMultiTouch&);  // Not implemented.
};

#endif
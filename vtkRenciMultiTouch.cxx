/*=========================================================================

  Name:        vtkRenciMultiTouch.cxx

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/

#include "vtkRenciMultiTouch.h"

#include "vtkObjectFactory.h"
#include "vtkstd/string"
#include "vtkstd/vector"

class vtkRenciMultiTouchInternals
{
public:
  vtkstd::string GestureName;
  vtkstd::vector<TouchPoint> TouchPoints;
};

vtkCxxRevisionMacro(vtkRenciMultiTouch, "$Revision: 1.0 $");
vtkStandardNewMacro(vtkRenciMultiTouch);

//----------------------------------------------------------------------------
vtkRenciMultiTouch::vtkRenciMultiTouch() 
{
  this->Internals = new vtkRenciMultiTouchInternals();

  this->HostName = NULL;
  this->Port = -1;
  this->SocketDescriptor = 0;
}

//----------------------------------------------------------------------------
vtkRenciMultiTouch::~vtkRenciMultiTouch() 
{
  this->SetHostName(NULL);
#ifdef WIN32
  closesocket(this->SocketDescriptor);
#else
  vtkErrorMacro(<<"Multi-touch currently only implemented on 32-bit Windows");
#endif

  delete this->Internals;
}

//----------------------------------------------------------------------------
int vtkRenciMultiTouch::Initialize() 
{
 // Initialize the socket library
#ifdef WIN32
  WORD versionRequested;
  WSADATA wsaData;

  // Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
  versionRequested = MAKEWORD(2, 2);

  if (WSAStartup(versionRequested, &wsaData) != 0) 
    {
    // Tell the user that we could not find a usable Winsock DLL
    vtkErrorMacro(<<"WSAStartup failed.");
    return 0;
    }
#else
  vtkErrorMacro(<<"Multi-touch currently only implemented on 32-bit Windows");
#endif

  // Check that we have a server to connect to
  if (HostName == NULL)
    {
    vtkErrorMacro(<<"Host name not set.");
    return 0;
    } 
  if (Port == -1)
    {
    vtkErrorMacro(<<"Port not set.");
    return 0;
    } 

  // Try to create the socket
  if (this->CreateSocket() != 0)
    {
    vtkErrorMacro(<<"Could not create socket!");
    return 0;
    }

  return 1;
}

//----------------------------------------------------------------------------
void vtkRenciMultiTouch::Update() 
{
  this->ClearGesture();

  // Read from the socket
  const int bufferSize = 16384;   // Magic number, taken from OSC MAX_UDP_PACKET_SIZE
  char buffer[bufferSize];
  memset(buffer, '\0', bufferSize);

  int numBytes = this->Receive(buffer, bufferSize);

  if (numBytes <= 0 || numBytes > bufferSize)
    {
    return;
    }

  // Tokenize
  this->ParseBuffer(buffer, numBytes);
}

//----------------------------------------------------------------------------
void vtkRenciMultiTouch::InvokeInteractionEvent() 
{
  if (this->Internals->GestureName == "") return;

  if (this->Internals->GestureName == "one_touch")
    {
    this->InvokeEvent(vtkRenciMultiTouch::OneTouchEvent,NULL);
    }
  else if (this->Internals->GestureName == "one_drag")
    {
    this->InvokeEvent(vtkRenciMultiTouch::OneDragEvent,NULL);
    }
  else if (this->Internals->GestureName == "two_touch")
    {
    this->InvokeEvent(vtkRenciMultiTouch::TwoTouchEvent,NULL);
    }
  else if (this->Internals->GestureName == "two_drag")
    {
    this->InvokeEvent(vtkRenciMultiTouch::TwoDragEvent,NULL);
    }
  else if (this->Internals->GestureName == "three_touch")
    {
    this->InvokeEvent(vtkRenciMultiTouch::ThreeTouchEvent,NULL);
    }
  else if (this->Internals->GestureName == "three_drag")
    {
    this->InvokeEvent(vtkRenciMultiTouch::ThreeDragEvent,NULL);
    }
  else if (this->Internals->GestureName == "four_touch")
    {
    this->InvokeEvent(vtkRenciMultiTouch::FourTouchEvent,NULL);
    }
  else if (this->Internals->GestureName == "four_drag")
    {
    this->InvokeEvent(vtkRenciMultiTouch::FourDragEvent,NULL);
    }
  else if (this->Internals->GestureName == "five_touch")
    {
    this->InvokeEvent(vtkRenciMultiTouch::FiveTouchEvent,NULL);
    }
  else if (this->Internals->GestureName == "five_drag")
    {
    this->InvokeEvent(vtkRenciMultiTouch::FiveDragEvent,NULL);
    }
  else if (this->Internals->GestureName == "six_touch")
    {
    this->InvokeEvent(vtkRenciMultiTouch::SixTouchEvent,NULL);
    }
  else if (this->Internals->GestureName == "six_drag")
    {
    this->InvokeEvent(vtkRenciMultiTouch::SixDragEvent,NULL);
    }
  else if (this->Internals->GestureName == "zoom")
    {
    this->InvokeEvent(vtkRenciMultiTouch::ZoomEvent,NULL);
    }
  else if (this->Internals->GestureName == "translate_x")
    {
    this->InvokeEvent(vtkRenciMultiTouch::TranslateXEvent,NULL);
    }
  else if (this->Internals->GestureName == "translate_y")
    {
    this->InvokeEvent(vtkRenciMultiTouch::TranslateYEvent,NULL);
    }
  else if (this->Internals->GestureName == "translate_z")
    {
    this->InvokeEvent(vtkRenciMultiTouch::TranslateZEvent,NULL);
    }
  else if (this->Internals->GestureName == "about_X_axis")
    {
    this->InvokeEvent(vtkRenciMultiTouch::RotateYEvent,NULL);
    }
  else if (this->Internals->GestureName == "about_Y_axis")
    {
    this->InvokeEvent(vtkRenciMultiTouch::RotateYEvent,NULL);
    }
  else if (this->Internals->GestureName == "about_Z_axis")
    {
    this->InvokeEvent(vtkRenciMultiTouch::RotateZEvent,NULL);
    }
  else if (this->Internals->GestureName == "release")
    {
    this->InvokeEvent(vtkRenciMultiTouch::ReleaseEvent,NULL);
    }
}

//----------------------------------------------------------------------------  
int vtkRenciMultiTouch::GetNumberOfTouchPoints()
{
  return this->Internals->TouchPoints.size();
}

//----------------------------------------------------------------------------  
const TouchPoint& vtkRenciMultiTouch::GetTouchPoint(int which)
{
  return this->Internals->TouchPoints[which];
}

//----------------------------------------------------------------------------
void vtkRenciMultiTouch::ParseBuffer(char* buffer, int numBytes)
{
  // Create a vtkStdString to make parsing certain things easier
  vtkstd::string s(buffer, numBytes);

  // Flag for the end of osc header junk
  const vtkstd::string start_flag = "set";

  // Find our start position  
  unsigned int si = 0;
  while (si < s.size() - start_flag.size() && (s.substr(si, start_flag.size())).compare(start_flag) != 0)
    {
    si++;
    }

  // Skip past start_flag
  si += (int)start_flag.size() + 1;

  vtkstd::string str = s.substr(si);

  // Find the gesture type
  this->Internals->GestureName = str.substr(0, str.find_first_of('\0'));

  if (this->Internals->GestureName == "release")
    {
    // No more info.  Return.
    return;
    }

  // Skip to next 4 byte chunk after the gesture name (including '\0')
  int chunkDist = 4 - ((int)this->Internals->GestureName.size() + 1) % 4;
  if (chunkDist == 4) chunkDist = 0;
  char* bufPtr = buffer + si + this->Internals->GestureName.size() + 1 + chunkDist;

  // Get the number of touches
  int numTouches = this->ReadInt(&bufPtr);

  for (int i = 0; i < numTouches; i++) 
    {
    TouchPoint tp;

    // Fill in the touch point
    tp.Id = this->ReadInt(&bufPtr);
    tp.Location[0] = this->ReadDouble(&bufPtr);
    tp.Location[1] = this->ReadDouble(&bufPtr);
    tp.Direction[0] = this->ReadDouble(&bufPtr);
    tp.Direction[1] = this->ReadDouble(&bufPtr);
    tp.MoveLocation = this->ReadInt(&bufPtr);

    this->Internals->TouchPoints.push_back(tp);
    }
}

//----------------------------------------------------------------------------
void vtkRenciMultiTouch::ClearGesture() 
{
  this->Internals->GestureName = "";
  this->Internals->TouchPoints.clear();
}

//----------------------------------------------------------------------------
int vtkRenciMultiTouch::CreateSocket()
{
#ifdef WIN32
  // Create a UDP socket
  this->SocketDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (this->SocketDescriptor == INVALID_SOCKET)
    {
    vtkErrorMacro(<<"Could not create socket!");
    return -1;
    }

  // Make non-blocking
  u_long blocking = 1;
  if (ioctlsocket(this->SocketDescriptor, FIONBIO, &blocking) == SOCKET_ERROR)
    {
    vtkErrorMacro(<<"Could not set non-blocking mode!");
    closesocket(this->SocketDescriptor);
    return -1;
    }

  // Set up the server information
  struct sockaddr_in server = {0};
  server.sin_family = AF_INET;
  server.sin_port = htons(this->Port);
  server.sin_addr.s_addr = INADDR_ANY;

  // Bind the address to the socket
  if (bind(this->SocketDescriptor, reinterpret_cast<struct sockaddr*>(&server), sizeof(sockaddr)) == SOCKET_ERROR)
    {
    vtkErrorMacro(<<"Could not bind name to socket!");
    closesocket(this->SocketDescriptor);
    return -1;
    }
#else
  vtkErrorMacro(<<"Multi-touch currently only implemented on 32-bit Windows");
#endif

  return 0;
}

//----------------------------------------------------------------------------
int vtkRenciMultiTouch::Receive(void* data, int length)
{
#ifdef WIN32
  return recvfrom(this->SocketDescriptor, (char*)data, length, 0, 0, 0);
#else
  vtkErrorMacro(<<"Multi-touch currently only implemented on 32-bit Windows");
  return 0;
#endif
}

//----------------------------------------------------------------------------
int vtkRenciMultiTouch::ReadInt(char** buffer)
{
  char intBuffer[4];
  memcpy(intBuffer, *buffer, 4);

  *buffer += 4;

  return ntohl(*((int*)intBuffer));
}

//----------------------------------------------------------------------------
double vtkRenciMultiTouch::ReadDouble(char** buffer)
{
  char doubleBuffer[8];
  memcpy(doubleBuffer, *buffer, 8);

  *buffer += 8;

  return this->Ntohd(*((double*)doubleBuffer));
}

//----------------------------------------------------------------------------
double vtkRenciMultiTouch::Ntohd(double d) 
{
  double swapped;
  char* chSwapped = (char*)&swapped;
  char* chD = (char*)&d;

  for (int i = 0; i < sizeof(double); i++) 
    {
    chSwapped[i] = chD[sizeof(double) - i - 1];
    }

  return swapped;
}

//----------------------------------------------------------------------------
void vtkRenciMultiTouch::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "HostName: " << this->HostName << "\n";
  os << indent << "Port: " << this->Port << "\n";
  os << indent << "SocketDescriptor: " << this->SocketDescriptor << "\n";
  os << indent << "GestureName: " << this->Internals->GestureName << "\n";
  os << indent << "TouchPoints:\n";
  for (unsigned int i = 0; i < this->Internals->TouchPoints.size(); i++)
    {
    os << indent << indent << "TouchPoint " << i << "\n";
    os << indent << indent << indent << "Id: " << this->Internals->TouchPoints[i].Id << "\n";
    os << indent << indent << indent << "Location: (" << this->Internals->TouchPoints[i].Location[0]
       << ", " << this->Internals->TouchPoints[i].Location[1] << ")\n";
    os << indent << indent << indent << "Direction: (" << this->Internals->TouchPoints[i].Direction[0]
       << ", " << this->Internals->TouchPoints[i].Direction[1] << ")\n";    
    os << indent << indent << indent << "MoveLocation " << this->Internals->TouchPoints[i].MoveLocation << "\n";    
    }
  os << "\n";
}

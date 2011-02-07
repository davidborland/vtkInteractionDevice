CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )

PROJECT( vtkInteractionDevice )

SET( LIBRARY_OUTPUT_PATH "${vtkInteractionDevice_BINARY_DIR}/lib" )


#######################################
# Include VTK
#######################################

FIND_PACKAGE( VTK REQUIRED )
INCLUDE( ${VTK_USE_FILE} ) 


#######################################
# Static or Shared Libraries
#######################################

# Shared libraries are necessary for wrapping Python
OPTION( BUILD_SHARED_LIBS
        "Build with shared libraries."
        ${VTK_BUILD_SHARED_LIBS} )
# Copy the CMake option to a setting with vtkInteractionDevice_ prefix for use in
# our project.  This name is used in vtkmsConfigure.h.in.
SET( vtkInteractionDevice_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS} )


#######################################
# Wrap Python
#######################################

IF( VTK_WRAP_PYTHON )

  OPTION( vtkInteractionDevice_WRAP_PYTHON
          "Wrap classes in Python."
          ON )

  IF( vtkInteractionDevice_WRAP_PYTHON )
    SET( VTK_WRAP_PYTHON3_INIT_DIR "${vtkInteractionDevice_SOURCE_DIR}/Wrapping" )
    INCLUDE( ${VTK_CMAKE_DIR}/vtkWrapPython.cmake )
    IF( WIN32 )
      IF( NOT BUILD_SHARED_LIBS )
        MESSAGE( FATAL_ERROR "Python support requires BUILD_SHARED_LIBS to be ON." )
        SET( vtkInteractionDevice_CAN_BUILD 0 )
      ENDIF( NOT BUILD_SHARED_LIBS )
    ENDIF( WIN32 )
  ENDIF( vtkInteractionDevice_WRAP_PYTHON )

ELSE( VTK_WRAP_PYTHON )

  IF( vtkInteractionDevice_WRAP_PYTHON )
    MESSAGE( "Warning. vtkInteractionDevice_WRAP_PYTHON is ON but the VTK version you have "
             "chosen does not support Python (VTK_WRAP_PYTHON is OFF).  "
             "Please set vtkInteractionDevice_WRAP_PYTHON to OFF." )
    SET( vtkInteractionDevice_WRAP_PYTHON OFF )
  ENDIF( vtkInteractionDevice_WRAP_PYTHON )

ENDIF( VTK_WRAP_PYTHON )


#######################################
# Configure file
#######################################

CONFIGURE_FILE( ${vtkInteractionDevice_SOURCE_DIR}/vtkInteractionDeviceConfigure.h.in
                ${vtkInteractionDevice_BINARY_DIR}/vtkInteractionDeviceConfigure.h )
                
INCLUDE_DIRECTORIES( ${vtkInteractionDevice_BINARY_DIR} )


#######################################
# Include vrpn
#######################################

FIND_PATH(VRPN_INCLUDE_DIR vrpn_Tracker.h)
FIND_LIBRARY(VRPN_LIBRARY vrpn)

INCLUDE_DIRECTORIES( ${VRPN_INCLUDE_DIR} )

#######################################
# Include vtkInteractionDevice code
#######################################

INCLUDE_DIRECTORIES( ${vtkInteractionDevice_SOURCE_DIR} )

SET( SRC vtkDeviceInteractor.h vtkDeviceInteractor.cxx
         vtkDeviceInteractorStyle.h vtkDeviceInteractorStyle.cxx
         vtkInteractionDevice.h vtkInteractionDevice.cxx
         vtkInteractionDeviceManager.h vtkInteractionDeviceManager.cxx
         vtkRenciMultiTouch.h vtkRenciMultiTouch.cxx
         vtkRenciMultiTouchStyle.h vtkRenciMultiTouchStyle.cxx
         vtkRenciMultiTouchStyleCamera.h vtkRenciMultiTouchStyleCamera.cxx
         vtkVRPNAnalog.h vtkVRPNAnalog.cxx
         vtkVRPNAnalogOutput.h vtkVRPNAnalogOutput.cxx
         vtkVRPNButton.h vtkVRPNButton.cxx
         vtkVRPNDevice.h vtkVRPNDevice.cxx
         vtkVRPNTracker.h vtkVRPNTracker.cxx
         vtkVRPNTrackerStyleCamera.h vtkVRPNTrackerStyleCamera.cxx
         vtkWiiMoteStyleCamera.h vtkWiiMoteStyleCamera.cxx
         vtkWiiMoteStyle.h vtkWiiMoteStyle.cxx )

IF (WIN32)
  SET( SRC ${SRC}
    vtkWin32RenderWindowDeviceInteractor.h vtkWin32RenderWindowDeviceInteractor.cxx )
ENDIF (WIN32)
ADD_LIBRARY( vtkInteractionDevice ${SRC} )
TARGET_LINK_LIBRARIES( vtkInteractionDevice
  vtkCommon
  vtkRendering
  ${VRPN_LIBRARY}
)

#######################################
# Create Python library
#######################################

IF( VTK_WRAP_PYTHON AND vtkInteractionDevice_WRAP_PYTHON)
  VTK_WRAP_PYTHON3( vtkInteractionDevicePython vtkInteractionDevicePython_SRC "${SRC}")
  ADD_LIBRARY( vtkInteractionDevicePythonD ${vtkInteractionDevicePython_SRC} )
  ADD_LIBRARY( vtkInteractionDevicePython MODULE vtkInteractionDevicePythonInit.cxx )
  TARGET_LINK_LIBRARIES( vtkInteractionDevicePythonD vtkInteractionDevice vtkRenderingPythonD )
  TARGET_LINK_LIBRARIES( vtkInteractionDevicePython vtkInteractionDevicePythonD )
ENDIF( VTK_WRAP_PYTHON AND vtkInteractionDevice_WRAP_PYTHON )


#######################################
# Include vtkInteractionDeviceTest
#######################################

ADD_SUBDIRECTORY( Test )
PROJECT( vtkInteractionDeviceTest ) 

SET( EXECUTABLE_OUTPUT_PATH "${vtkInteractionDevice_BINARY_DIR}/bin" )

SET( VTK_LIBS vtkRendering )

SET( SRC vtkInteractionDeviceTest )
ADD_EXECUTABLE( vtkInteractionDeviceTest MACOSX_BUNDLE ${SRC} )
ADD_DEPENDENCIES( vtkInteractionDeviceTest vtkInteractionDevice )
TARGET_LINK_LIBRARIES( vtkInteractionDeviceTest 
  vtkInteractionDevice
  ${VTK_LIBS}
  ${VRPN_LIBRARY}
)

MESSAGE( ${VRPN_LIBRARY} )

PROJECT( vtkInteractionDeviceTest ) 

SET( EXECUTABLE_OUTPUT_PATH "${vtkInteractionDevice_BINARY_DIR}/bin" )

SET( VTK_LIBS vtkRendering )

SET( SRC vtkInteractionDeviceTest )
ADD_EXECUTABLE( vtkInteractionDeviceTest MACOSX_BUNDLE ${SRC} )
ADD_DEPENDENCIES( vtkInteractionDeviceTest vtkInteractionDevice )
TARGET_LINK_LIBRARIES( vtkInteractionDeviceTest 
                       ${VTK_LIBS}
                 debug ${vtkInteractionDevice_BINARY_DIR}/lib/debug/vtkInteractionDevice.lib ${VRPN_DIR}/pc_win32/Debug/vrpn.lib
             optimized ${vtkInteractionDevice_BINARY_DIR}/lib/release/vtkInteractionDevice.lib ${VRPN_DIR}/pc_win32/Release/vrpn.lib )
# vtkInteractionDevice
Enables VTK applications to interface with external interaction devices via libraries such as VRPN 

Python notes:  
* Should only build in Release mode.  
* When wrapping Python on Windows, need to change vtkInteractionDevice.dll to vtkInteractionDevice.pyd and add the containing directory to the PYTHONPATH environment variable.

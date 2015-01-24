# Air Pollution - Visualization of the Air Quality

In summer 2014, the Public Health Department of National City (California) sent out teams to take air quality measurements throughout National City. This project tries to visualize the measurements by rendering smoke with varying density according to the air quality onto a map of National City.

![Screenshot](https://github.com/FelixHoer/AirPollution/raw/master/screenshot.jpg)

## Technical

From a dataset of around 15.000 entries the locations where the teams stayed for a minute or longer are selected. At those locations a particle system is rendered. The particles are translucent quads, which have a smoke cloud texture on them. A level of detail method is used to adapt the number of particles based on the distance of the viewer.

The map consists of 1024 tiles that were extracted from OpenStreetMap. Those tiles are arranged in a quad-tree to optimize view-frustum checks.

The implementation makes heavy use of a scene-graph and view-frustum culling, to minimize the number of map-tiles and particle systems that have to be rendered.

## Requirements

* glut
* glm
* glew

## Configuration

`Copy dlls to system directory`
* `C:\glew-1.11.0-win32\glew-1.11.0\bin\Release\Win32\glew32.dll` to `C:\Windows\System`
* `glut32.dll` to `C:\Windows\System`

`C/C++ > General > Additional Include Directories`
* `C:\glut-3.7.6-bin\include`
* `C:\glm-0.9.5.4\glm`
* `C:\glew-1.11.0-win32\glew-1.11.0\include`

`Linker > General > Additional Library Directories`
* `C:\glut-3.7.6-bin\lib`
* `C:\glew-1.11.0-win32\glew-1.11.0\lib\Release\Win32`

`Linker > Input > Additional Dependencies`
* `C:\glut-3.7.6-bin\lib\glut32.lib`
* `C:\glew-1.11.0-win32\glew-1.11.0\lib\Release\Win32\glew32.lib`
* `C:\glew-1.11.0-win32\glew-1.11.0\lib\Release\Win32\glew32s.lib`

`C++ > Preprocessor > Preprocessor Definitions`
* `GLM_FORCE_RADIANS`
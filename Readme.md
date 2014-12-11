# Air Pollution

Visualization of the Air Pollution

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
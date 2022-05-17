# pcsp

![Minifire demo](https://github.com/georgemoralis/pcsp/blob/main/screenshot/minifiredemo.png) 

Test code for psp emulator

win32 build : interpeter and recompiler works (hmm recompiler is fast  , interpreter less than 1 fps)

win64 build : only interpreter 10% faster than win32 version

This can only run minifire demo which is embedded inside source , so running exe will run the demo automaticaly. There are several hacks to skip syscalls and run the demo with minimal emulation.

# TODO

* update opengl from v1 to v3 and use glfw as a step in for adding imgui 
* use imgui
* make it a proper psp emulator rather than hackish minifire demo port

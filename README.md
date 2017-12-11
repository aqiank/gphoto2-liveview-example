gPhoto2 LiveView Example
========================
This example shows one of the ways you can stream live video feed from your DSLR camera. It simply reads MJPEG output by gPhoto2 frame-by-frame, parses it, and put up the frame buffer on the screen window.

You should be able to easily change the graphical system to use something else (e.g. GLFW, Qt, openFrameworks, etc..)

Requirements
------------
1. [GCC](https://gcc.gnu.org)
3. [gPhoto2](http://www.gphoto.org/proj/gphoto2)
3. [SDL2](https://libsdl.org)
4. [SDL2_image](https://libsdl.org/projects/SDL_image)
5. DSLR Camera

All those packages should be available through your Linux or OSX package managers.

Setup
-----
1. Plug in a USB connector from your camera to your computer
2. (optional) Change WINDOW_WIDTH and WINDOW_HEIGHT as needed (if your camera outputs high-resolution videos)

Build
-----
1. Run `make`

Run
---
1a. To grab frames directly from camera, run `gphoto2 --capture-movie --stdout | ./gphoto2-liveview-example`  
1b. To use existing MJPEG recording, run `./gphoto2-liveview-example [MJPEG filename]`  

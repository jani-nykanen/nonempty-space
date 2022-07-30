## Nonempty Space


...is a software rendered 3D demo written in C. Its only goal is to demonstrate my rewritten software renderer. It's called "Nonempty Space" because it was supposed to be space-themed, and I didn't bother to rename it.


### Binaries


You can download pre-built binaries in the [Releases](https://github.com/jani-nykanen/nonempty-space/releases/tag/v1.0.0) section.


### Building


On Linux, just type `make` on the root and it should build a file called `application` for you. Make sure you have `glfw3` installed. On Windows... well, you might need to change the `makefile` a bit to get it working (replaced CC and LIBFOLDER variables with commented out values, replace LD_FLAGS with LD_FLAGS_WIN32) in `application` rule). I might eventually make it possible to (cross-)compile to Windows "automatically", but since I'm lazy... well, we'll see!


### License


Do whatever you want with this, just don't claim you made it


------


(c) 2022 Jani Nykänen

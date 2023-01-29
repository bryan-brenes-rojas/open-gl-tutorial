# OpenGL Tutorial

## Setup

```bash
sudo dnf install glfw glfw-devel
sudo dnf install glew libGLEW glew-devel
```

To be able to link the libraries when compiling options must be passed to the
compilation command. This can be obtained using:

```bash
pkg-config --libs glew glfw3 gl
```

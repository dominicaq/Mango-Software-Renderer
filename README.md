# PureC Software renderer
The **PureC Software Renderer** is an educational piece that provides a deep
dive into the inner workings of the 3D rendering pipeline. This project showcases
the rendering pipeline typically abstracted by graphics APIs like OpenGL, Vulkan,
etc. This project expands further and implements features that you would
typically do in a computer graphics course.

This renderer is **not** designed for real-world use because it is CPU bound.
A typical rendering API will use the GPU.

# Features
- Rendering of 3D GameObjects
- Loading triangulated OBJ files
- Vertex / Fragment Shader pipeline
- Model View Perspective transformations (MVP)
- Blinn-Phong shader for realistic lighting

# Prerequisites
- **CMake 3.27.7** A cross-platform build system. You can download it [here.](https://cmake.org/download/)

# Build
Once you have CMake installed, follow these steps to build the project:
```console
cd software_renderer
mkdir build
cd build
cmake ..
```

# Run
Once built, you can run the software renderer as follows:
```console
.\software_renderer
```

# License
This project is licensed under the [MIT License.]

All model assets are licensed under the [??? License.]
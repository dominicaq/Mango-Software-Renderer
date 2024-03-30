# Mango Software Renderer
The **Mango Software Renderer** is an educational piece that provides a deep
dive into the inner workings of the 3D rendering pipeline. This project showcases
the rendering pipeline typically abstracted by graphics APIs like OpenGL, Vulkan,
etc. This project expands further and implements features that you would
typically do in a computer graphics course.

<p align="center">
  <img width="640" height="360" src="https://github.com/dominicaq/Mango-Software-Renderer/blob/main/images/showcase.png">
</p>

# Disclaimer
This software renderer is **not** intended for real-world use due to its
CPU-bound nature. In contrast, modern rendering APIs leverage the GPU for high-performance computations.

# Features
- Rendering of 3D GameObjects
- Loading triangulated OBJ files
- Vertex / Fragment Shader pipeline
- View frustum clipping (on near plane)
- Model View Perspective transformations (MVP)
- Blinn-Phong shader for realistic lighting
- Multi-light fragment shader
- Signed Distance Fields

# Options
- View Normals
- View UV map
- Enable/Disable rasterization
- Wireframe
- Enable/Disable Signed Distance Field rendering (SDF)

<p align="center">
  <img width="640" height="360" src="https://github.com/dominicaq/Mango-Software-Renderer/blob/main/images/render_options.gif">
</p>

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
This project is licensed under the [MIT License.](https://github.com/dominicaq/PureC-Software-renderer/blob/main/LICENSE)

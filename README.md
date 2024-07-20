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
- View frustum clipping
- Model View Perspective transformations (MVP)
- Blinn-Phong shader for realistic lighting
- Multi-light fragment shader
- Texture Sampling

# Options
- Wireframe
- View Normals
- View UV map
- View only textures
- View only colors
- View only lighting
- Enable/Disable rasterization
- Enable/Disable Signed Distance Field rendering (SDF) (WIP)

<p align="center">
  <img width="800" height="600" src="https://github.com/dominicaq/Mango-Software-Renderer/blob/main/images/render_options.gif">
</p>

# Prerequisites
- **CMake 3.27.7** A cross-platform build system. You can download it [here.](https://cmake.org/download/)

# Build
Once you have CMake installed, follow these steps to build the project:
```console
cd Mango-Software-Renderer
git submodule update --init --recursive
mkdir build
cd build
cmake ..
```

# Run
Once built, you can run the renderer in the build folder as follows:
```console
.\mango_renderer
```

# License
This project is licensed under the [MIT License.](https://github.com/dominicaq/PureC-Software-renderer/blob/main/LICENSE)

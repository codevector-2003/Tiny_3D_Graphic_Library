# Tiny3D Graphics Library

A lightweight 3D graphics library written in C++ for rendering wireframe models with lighting and animation support.

## Features

- **3D Math Library**: Vectors, matrices, transformations
- **Rendering Pipeline**: Vertex projection, wireframe rendering, depth sorting
- **Lighting System**: Lambert shading with multiple light sources
- **Animation System**: Bezier curves, time-based animations
- **Canvas System**: Floating-point pixel rendering with bilinear filtering

## Documentation

For detailed documentation, see the [Tiny3D Graphics Driver PDF](libtiny3d/documentation/labtiny3d%20graphic%20driver.pdf).

## Live Demo

### Eiffel Tower Wireframe Animation

<video width="600" controls>
  <source src="libtiny3d/Captures/eiffel_tower.mp4" type="video/mp4">
  Your browser does not support the video tag. <a href="libtiny3d/Captures/eiffel_tower.mp4">Download video</a>
</video>

*Detailed Eiffel Tower with extensive lattice structure and cross-bracing, rotating on Y-axis to showcase all features.*

### Chess King Rotation

<video width="600" controls>
  <source src="libtiny3d/Captures/chess_king.mp4" type="video/mp4">
  Your browser does not support the video tag. <a href="libtiny3d/Captures/chess_king.mp4">Download video</a>
</video>

*Detailed chess king piece with octagonal base, crown with alternating points, and cross on top.*

### Rotating Sphere Wireframe

<video width="600" controls>
  <source src="libtiny3d/Captures/sphere_animated.mp4" type="video/mp4">
  Your browser does not support the video tag. <a href="libtiny3d/Captures/sphere_animated.mp4">Download video</a>
</video>

*3D sphere with latitude/longitude wireframe structure, smoothly rotating on Y-axis.*

### More Demo Videos

- [Polyhedra Shapes](libtiny3d/Captures/polyhedra.mp4) - Tetrahedron and octahedron

### How to Record Demo Videos

1. Run any test program (e.g., `.\build\bin\test12_eiffel.exe`)
2. Use screen recording software like:
   - **Windows Game Bar** (Win + G)
   - **OBS Studio** (free)
   - **ShareX** (free)
3. Save recordings as MP4 files in `libtiny3d/Captures/` folder
4. Videos will display automatically in this README

## Project Structure

```
libtiny3d/
├── include/          # Header files
│   ├── animation.h
│   ├── canvas.h
│   ├── lighting.h
│   ├── math3d.h
│   └── renderer.h
├── src/             # Implementation files
│   ├── animation.cpp
│   ├── canvas.cpp
│   ├── lighting.cpp
│   ├── math3d.cpp
│   └── renderer.cpp
├── demo/            # Demo applications
│   └── main.cpp
├── tests/           # Test files
│   ├── test_animation.cpp
│   └── test_math.cpp
├── build/           # Build output (generated)
│   ├── obj/        # Object files
│   ├── lib/        # Static library
│   └── bin/        # Executables
├── Makefile         # Build configuration
└── README.md        # This file
```

## Building

### Prerequisites

- C++17 compatible compiler (g++, clang++)
- Make build system

### Build Commands

```bash
# Build everything (library, demos, tests)
make

# Build only the static library
make lib

# Build demo applications
make demo

# Build test executables
make tests

# Run all tests
make run-tests

# Run the demo
make run-demo

# Clean build artifacts
make clean

# Show help
make help
```

## Usage

### Linking Against the Library

After building, link your program against `build/lib/libtiny3d.a`:

```bash
g++ -std=c++17 -Iinclude your_program.cpp build/lib/libtiny3d.a -o your_program
```

### Basic Example

```cpp
#include "math3d.h"
#include "renderer.h"
#include "canvas.h"

int main() {
    // Create canvas
    Canvas canvas(800, 800);
    
    // Define cube vertices
    vec3_t vertices[8] = {
        {-1,-1,-1}, {1,-1,-1}, {1,1,-1}, {-1,1,-1},
        {-1,-1, 1}, {1,-1, 1}, {1,1, 1}, {-1,1, 1}
    };
    
    // Define edges
    int edges[12][2] = {
        {0,1},{1,2},{2,3},{3,0},
        {4,5},{5,6},{6,7},{7,4},
        {0,4},{1,5},{2,6},{3,7}
    };
    
    // Setup matrices
    mat4 model = mat4::translation(0, 0, -5);
    mat4 view = mat4::identity();
    mat4 projection = mat4::frustumAssymetric(-1, 1, -1, 1, 1, 50);
    
    // Render
    renderer_wireframe(canvas, vertices, 8, edges, 12,
                      model, view, projection, 800, 800);
    
    return 0;
}
```

## API Overview

### Math3D (`math3d.h`)

- `vec3_t`: 3D vector with x, y, z components
- `mat4`: 4x4 transformation matrix
- Matrix operations: translation, rotation, scaling, projection
- Vector operations: dot product, cross product, normalization

### Renderer (`renderer.h`)

- `renderer_wireframe()`: Render wireframe models with depth sorting
- `project_vertex()`: Transform vertices through the graphics pipeline
- Circular viewport clipping

### Canvas (`canvas.h`)

- `Canvas`: Framebuffer with floating-point pixel values
- `set_pixel_f()`: Set pixel with bilinear filtering
- `draw_line_f()`: Draw anti-aliased lines

### Lighting (`lighting.h`)

- `Light`: Directional light with intensity
- `lambert_edge()`: Calculate edge lighting
- `lambert_edge_multi()`: Multi-light edge lighting

### Animation (`animation.h`)

- `bezier()`: Cubic Bezier curve evaluation
- `loop_time()`: Time loop helper

## License

This project is provided as-is for educational purposes.

## Contributing

Contributions are welcome! Please ensure code follows the existing style and includes appropriate tests.

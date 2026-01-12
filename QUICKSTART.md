# Quick Start - Visualizing 3D Graphics

## What's New

I've added a **visualization system** that displays animated 3D objects right in your console window!

### New Features:

1. **ASCII Display System** - View 3D graphics in your terminal
2. **Interactive Demo** - Watch multiple 3D objects move and rotate in real-time
3. **Bezier Path Animation** - Objects follow smooth curved paths
4. **Multiple Objects** - Cube, pyramid, and more spinning together

## Quick Start

### Step 1: Install a C++ Compiler

Choose ONE option:

**Option A: MinGW (Recommended - Lightweight)**
```powershell
# Install via Chocolatey (run as Administrator)
choco install mingw -y
```

**Option B: Visual Studio**
- Download from: https://visualstudio.microsoft.com/downloads/
- Install "Desktop development with C++"

### Step 2: Build the Project

```powershell
cd "C:\Users\SAMPR\OneDrive\Desktop\GitHub Projects\C-graphic-driver\libtiny3d"
.\build-simple.ps1
```

Or if you prefer batch files:
```batch
.\build-simple.bat
```

### Step 3: Run the Interactive Demo

```powershell
.\build\bin\interactive.exe
```

## What You'll See

The interactive demo shows:

- **Rotating Cube** - Following a curved path (top)
- **Spinning Pyramid** - Following another path (bottom)
- **Center Cube** - Rotating in place at the center
- **Real-time Animation** - Smooth 60 FPS motion
- **ASCII Art Rendering** - 3D wireframes displayed using text characters

## Demo Features

### Objects Included:
- Cube (8 vertices, 12 edges)
- Pyramid (5 vertices, 8 edges)

### Animation Features:
- **Bezier Curves** - Smooth curved motion paths
- **Rotation** - Objects spin on multiple axes
- **Path Following** - Objects move along predefined curves
- **Depth Sorting** - Proper wireframe rendering

### Display:
- **Resolution**: 800x800 internal canvas
- **Console Output**: 120x40 character display
- **Grayscale**: 65-level ASCII grayscale for depth
- **Frame Rate**: ~60 FPS

## Controls

Currently the demo runs automatically. Press **Ctrl+C** to exit.

## Customization

Want to add your own objects? Edit `demo/interactive.cpp`:

### Add a New Shape:

```cpp
// Define vertices
vec3_t my_shape_vertices[4] = {
    {0, 1, 0},   // Top
    {-1, -1, 0}, // Bottom left
    {1, -1, 0},  // Bottom right
    {0, -1, 2}   // Back
};

// Define edges
const int my_shape_edges[6][2] = {
    {0,1}, {0,2}, {0,3},
    {1,2}, {2,3}, {3,1}
};

// In the render loop:
mat4 modelD = multiply(
    mat4::translation(2, 0, -7),  // Position
    mat4::rotation_xyz(t, t * 2, 0)  // Rotation
);

renderer_wireframe(
    canvas,
    my_shape_vertices, 4,
    my_shape_edges, 6,
    modelD, view, projection,
    SCREEN_W, SCREEN_H
);
```

### Change Animation Paths:

```cpp
// Modify the Bezier control points
vec3_t pathA0(-5, 2, -8);   // Start
vec3_t pathA1(-5, -2, -8);  // Control 1
vec3_t pathA2( 5, -2, -8);  // Control 2
vec3_t pathA3( 5, 2, -8);   // End
```

### Adjust Display Size:

```cpp
const int DISPLAY_W = 120;  // Width in characters
const int DISPLAY_H = 40;   // Height in characters
```

## File Structure

```
demo/
├── main.cpp           - Original static demo
├── interactive.cpp    - NEW: Animated 3D demo

include/
├── display.h          - NEW: Display system header

src/
├── display.cpp        - NEW: ASCII display implementation
```

## Next Steps

1. **Add More Shapes** - Create complex 3D models
2. **Interactive Input** - Add keyboard controls
3. **Camera Movement** - Implement camera rotation
4. **Color Support** - Use ANSI colors in terminal
5. **Export to Image** - Save frames as image files

## Troubleshooting

**Demo runs too fast/slow?**
- Adjust the sleep time in the render loop
- Change: `int sleep_time = 16 - (int)elapsed.count();` (16ms = ~60 FPS)

**ASCII art looks weird?**
- Use Windows Terminal or PowerShell 7+ for best results
- Try adjusting `DISPLAY_W` and `DISPLAY_H` to fit your console

**Want smooth graphics?**
- Maximize your console window
- Use a monospace font (Consolas, Courier New)

## Have Fun!

Experiment with different shapes, paths, and animations. The library is yours to explore!

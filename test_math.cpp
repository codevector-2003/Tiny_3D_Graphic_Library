#include <iostream>
#include <iomanip>
#include "math3d.h"

// Helper to print vec3
void print_vec3(const char* label, const vec3_t& v)
{
    std::cout << label << " ("
              << std::fixed << std::setprecision(3)
              << v.x << ", "
              << v.y << ", "
              << v.z << ")\n";
}

int main()
{
    std::cout << "=== 3D Math Engine Test ===\n\n";

    //Define a cube in OBJECT SPACE (centered at origin)
    vec3_t cube[8] = {
        {-1, -1, -1},
        { 1, -1, -1},
        { 1,  1, -1},
        {-1,  1, -1},
        {-1, -1,  1},
        { 1, -1,  1},
        { 1,  1,  1},
        {-1,  1,  1}
    };

    //Build MODEL matrix (scale → rotate → translate)
    mat4 scale = mat4::scale(1.0f, 1.0f, 1.0f);
    mat4 rotate = mat4::rotation_xyz(0.5f, 0.5f, 0.0f); // radians
    mat4 translate = mat4::translation(0.0f, 0.0f, -5.0f);

    mat4 model = multiply(translate, multiply(rotate, scale));

    // VIEW matrix (identity = camera at origin)
    mat4 view = mat4::identity();

    //PROJECTION matrix (asymmetric frustum)
    mat4 projection = mat4::frustumAssymetric(
        -1.0f, 1.0f,   // left, right
        -1.0f, 1.0f,   // bottom, top
         1.0f, 100.0f  // near, far
    );

    // Combine MVP
    mat4 vp = multiply(projection, view);
    mat4 mvp = multiply(vp, model);

    // 5Transform cube vertices
    std::cout << "Projected cube vertices:\n\n";

    for (int i = 0; i < 8; ++i)
    {
        vec3_t v = cube[i];

        // Apply MVP transform
        vec3_t clip = multiply(mvp, v);

        // Perspective divide (manual!)
        // NOTE: Your multiply must internally compute w.
        // If not, z acts as depth proxy in this test.
        float w = 1.0f; // assumed

        clip.x /= w;
        clip.y /= w;
        clip.z /= w;

        std::cout << "Vertex " << i << ": ";
        print_vec3("", clip);
    }

    // 6️⃣ SLERP TEST (direction interpolation)
    std::cout << "\nSLERP test:\n";

    vec3_t dirA(1, 0, 0);
    vec3_t dirB(0, 0, -1);

    dirA.normalize_fast();
    dirB.normalize_fast();

    for (float t = 0; t <= 1.01f; t += 0.25f)
    {
        vec3_t d = vec3_t::slerp(dirA, dirB, t);
        std::cout << "t = " << t << " ";
        print_vec3("dir", d);
    }

    std::cout << "\n=== Test Complete ===\n";
    return 0;
}

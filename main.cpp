#include "canvas.h"
#include <iostream>

int main()
{
    Canvas c(200, 200);

    // Thin line
    draw_line_f(c, 20, 20, 180, 100, 1.0f, 1.0f);
    printf("success\n");
    // Thick line
    draw_line_f(c, 20, 100, 180, 180, 6.0f, 1.0f);
    printf("success\n");
    return 0;
}

#ifndef DISPLAY_H
#define DISPLAY_H

#include "canvas.h"

/* ASCII Display - Works in any console (no dependencies) */
class ASCIIDisplay
{
public:
    ASCIIDisplay(int width, int height);
    ~ASCIIDisplay();

    void show(const Canvas &canvas);
    void clear();

private:
    int width;
    int height;
};

#endif

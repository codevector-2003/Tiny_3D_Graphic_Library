#ifndef WINDOW_DISPLAY_H
#define WINDOW_DISPLAY_H

#include "canvas.h"
#include <windows.h>

/* Windows GDI Display - Renders in a proper window */
class WindowDisplay
{
public:
    WindowDisplay(int width, int height, const char *title = "Tiny3D Graphics");
    ~WindowDisplay();

    void show(const Canvas &canvas);
    bool is_open();
    void process_events();

private:
    HWND hwnd;
    HDC hdc;
    HDC hdcMem;
    HBITMAP hBitmap;
    HGDIOBJ oldBitmap;
    void *bitmapData;
    int width;
    int height;
    bool open;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif

#include "window_display.h"
#include <algorithm>

WindowDisplay::WindowDisplay(int w, int h, const char *title)
    : width(w), height(h), open(true), hBitmap(NULL), bitmapData(NULL)
{
    // Register window class
    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "Tiny3DWindow";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    // Only register if not already registered
    if (!GetClassInfoA(GetModuleHandle(NULL), "Tiny3DWindow", &wc))
    {
        RegisterClassA(&wc);
    }

    // Create window
    hwnd = CreateWindowExA(
        0,
        "Tiny3DWindow",
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width + 16, height + 39, // Account for window borders
        NULL,
        NULL,
        GetModuleHandle(NULL),
        NULL);

    // Set user data BEFORE showing window
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    // Create DIB for rendering
    hdc = GetDC(hwnd);

    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height; // Top-down
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &bitmapData, NULL, 0);

    // Create persistent memory DC
    hdcMem = CreateCompatibleDC(hdc);
    oldBitmap = SelectObject(hdcMem, hBitmap);
}

WindowDisplay::~WindowDisplay()
{
    if (hdcMem)
    {
        SelectObject(hdcMem, oldBitmap);
        DeleteDC(hdcMem);
    }
    if (hBitmap)
        DeleteObject(hBitmap);
    if (hdc)
        ReleaseDC(hwnd, hdc);
    if (hwnd)
        DestroyWindow(hwnd);
}

bool WindowDisplay::is_open()
{
    return open;
}

void WindowDisplay::process_events()
{
    MSG msg;
    // Process ALL pending messages to keep window responsive
    // Use NULL instead of hwnd to process all thread messages
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            open = false;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void WindowDisplay::show(const Canvas &canvas)
{
    if (!bitmapData || !open || !hwnd)
        return;

    // Convert canvas to BGRA format - optimized
    unsigned int *pixels = (unsigned int *)bitmapData;
    int minHeight = (height < canvas.height) ? height : canvas.height;
    int minWidth = (width < canvas.width) ? width : canvas.width;

    for (int y = 0; y < minHeight; y++)
    {
        float *row = canvas.pixels[y];
        unsigned int *pixelRow = pixels + (y * width);

        for (int x = 0; x < minWidth; x++)
        {
            float intensity = row[x];

            // Clamp and convert to grayscale
            if (intensity < 0.0f)
                intensity = 0.0f;
            if (intensity > 1.0f)
                intensity = 1.0f;
            unsigned char value = (unsigned char)(intensity * 255.0f);

            // BGRA format
            pixelRow[x] = (value << 16) | (value << 8) | value;
        }

        // Process events every 50 rows to keep responsive during conversion
        if (y % 50 == 0)
        {
            MSG msg;
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                    open = false;
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

    // Blit to window
    BitBlt(hdc, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);
    GdiFlush();

    // Validate to prevent excessive repaints
    ValidateRect(hwnd, NULL);
}

LRESULT CALLBACK WindowDisplay::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WindowDisplay *display = (WindowDisplay *)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    switch (uMsg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        if (display && display->hdcMem)
        {
            BitBlt(hdc, 0, 0, display->width, display->height, display->hdcMem, 0, 0, SRCCOPY);
        }
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_ERASEBKGND:
        return 1; // Prevent flicker

    case WM_CLOSE:
        if (display)
            display->open = false;
        PostQuitMessage(0);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

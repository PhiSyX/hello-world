#include <windows.h>

// @see https://docs.microsoft.com/en-us/windows/win32/api/winuser/

#define internal static
#define local_persist static
#define global_variable static

// TODO: this is a global variable for now.

global_variable bool running;

global_variable BITMAPINFO bitmap_info;
global_variable void* bitmap_memory;
global_variable HBITMAP bitmap_handle;
global_variable HDC bitmap_device_context;

internal void
win32_resize_di_section(int w, int h)
{
  // TODO: bulletproof this.
  // Maybe don't free first, free after, then free first if that fails.

  if (bitmap_handle) {
    DeleteObject(bitmap_handle);
  }

  if (bitmap_device_context) {
    // TODO: should we recreate these under certain special circumstances
    bitmap_device_context = CreateCompatibleDC(0);
  }

  bitmap_info.bmiHeader.biSize = sizeof(bitmap_info.bmiHeader);
  bitmap_info.bmiHeader.biWidth = w;
  bitmap_info.bmiHeader.biHeight = h;
  bitmap_info.bmiHeader.biPlanes = 1;
  bitmap_info.bmiHeader.biBitCount = 32;
  bitmap_info.bmiHeader.biCompression = BI_RGB;

  CreateDIBSection(
    bitmap_device_context, &bitmap_info, DIB_RGB_COLORS, &bitmap_memory, 0, 0);
}

internal void
win32_update_window(HDC device_context, int x, int y, int w, int h)
{
  StretchDIBits(device_context,
                x,
                y,
                w,
                h,
                x,
                y,
                w,
                h,
                &bitmap_memory,
                &bitmap_info,
                DIB_RGB_COLORS,
                SRCCOPY);
}

LRESULT CALLBACK
MainWindowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
  LRESULT result = 0;

  switch (message) {
    case WM_SIZE: {
      RECT client_rect;
      GetClientRect(window, &client_rect);

      auto w = client_rect.right - client_rect.left;
      auto h = client_rect.bottom - client_rect.top;

      win32_resize_di_section(w, h);
      OutputDebugStringA("WM_SIZE\n");
    } break;

    case WM_DESTROY: {
      OutputDebugStringA("WM_DESTROY\n");
      PostQuitMessage(0);
    } break;

    case WM_CLOSE: {
      OutputDebugStringA("WM_CLOSE\n");
      PostQuitMessage(0);
    } break;

    case WM_ACTIVATEAPP: {
      OutputDebugStringA("WM_ACTIVATEAPP\n");
    } break;

    case WM_PAINT: {
      OutputDebugStringA("WM_PAINT\n");
      PAINTSTRUCT paint;

      HDC device_context = BeginPaint(window, &paint);

      auto w = paint.rcPaint.right - paint.rcPaint.left;
      auto h = paint.rcPaint.bottom - paint.rcPaint.top;
      auto x = paint.rcPaint.left;
      auto y = paint.rcPaint.top;

      //   PatBlt(device_context, x, y, w, h, WHITENESS);

      win32_update_window(device_context, x, y, w, h);

      EndPaint(window, &paint);
    } break;

    default: {
      result = DefWindowProc(window, message, wParam, lParam);
    } break;
  }

  return result;
}

int CALLBACK
WinMain(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow)
{
  WNDCLASS win_class = {};

  // todo: check if HREDRAW/VREDRAW/OWNDC still matter
  win_class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
  win_class.lpfnWndProc = DefWindowProc;
  win_class.hInstance = hInstance;
  win_class.lpszClassName = "CppWindowClass";

  if (RegisterClass(&win_class)) {
    HWND window_handle = CreateWindowExA(0,
                                         win_class.lpszClassName,
                                         "Cpp window",
                                         WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                         CW_USEDEFAULT,
                                         CW_USEDEFAULT,
                                         CW_USEDEFAULT,
                                         CW_USEDEFAULT,
                                         NULL,
                                         NULL,
                                         hInstance,
                                         NULL);

    if (window_handle) {
      MSG message;
      while (GetMessage(&message, NULL, 0, 0) != 0) {
        TranslateMessage(&message);
        DispatchMessage(&message);
      }
    }
  } else {
    // todo: Logging
  }

  return 0;
}

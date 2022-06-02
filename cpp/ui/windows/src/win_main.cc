#include <windows.h>
#include <stdint.h>

// @see https://docs.microsoft.com/en-us/windows/win32/api/winuser/

#define internal        static
#define local_persist   static
#define global_variable static

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

// TODO: this is a global variable for now.

global_variable bool running;

global_variable BITMAPINFO  bitmap_info;
global_variable void*       bitmap_memory;
global_variable int         bitmap_w;
global_variable int         bitmap_h;

global_variable int byte_per_pixel = 4;

internal void
render_weird_gradient(int offset_x, int offset_y) 
{
  int w = bitmap_w;
  int h = bitmap_h;
 
  int pitch = w * byte_per_pixel;
  u8 * row = (u8 *)bitmap_memory;
  for (int y = 0; y < bitmap_h; ++y) {
    u32 * pixel = (u32 *)row;

    for (int x = 0; x < bitmap_w; ++x) {
        u8 blue = (x + offset_x);
        u8 green = (y + offset_y);
        /*
          Memory:    BB GG RR xx
          Register:  xx RR GG BB
          
          Pixel 32-bits
        */
        *pixel++ = ((green << 8) | blue);
    }

    row += pitch;
  }
}

internal void
win32_resize_dib_section(int w, int h)
{
  // TODO: bulletproof this.
  // Maybe don't free first, free after, then free first if that fails.

  if (bitmap_memory) {
    VirtualFree(bitmap_memory, 0, MEM_RELEASE);
  }

  bitmap_w = w;
  bitmap_h = h;

  bitmap_info.bmiHeader.biSize        = sizeof(bitmap_info.bmiHeader);
  bitmap_info.bmiHeader.biWidth       = bitmap_w;
  bitmap_info.bmiHeader.biHeight      = -bitmap_h;
  bitmap_info.bmiHeader.biPlanes      = 1;
  bitmap_info.bmiHeader.biBitCount    = 32;
  bitmap_info.bmiHeader.biCompression = BI_RGB;

  int bitmap_memory_size = (bitmap_w * bitmap_h) * byte_per_pixel;
  bitmap_memory = VirtualAlloc(
    0,
    bitmap_memory_size,
    MEM_COMMIT,
    PAGE_READWRITE
  );
}

internal void
win32_update_window(HDC device_context, RECT *WindowRect,
                    int x, int y, int w, int h)
{
  int window_w = WindowRect->right - WindowRect->left;
  int window_h = WindowRect->bottom - WindowRect->top;
  StretchDIBits(device_context,
                0, 0, bitmap_w, bitmap_h,
                0, 0, window_w, window_h,
                bitmap_memory, &bitmap_info,
                DIB_RGB_COLORS, SRCCOPY);
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
      win32_resize_dib_section(w, h);
    } break;

    case WM_DESTROY: {
      // TODO: handle this with a message to the user.
      running = false;
      PostQuitMessage(0);
    } break;

    case WM_CLOSE: {
      // TODO: handle this with as an error - recreate window.
      running = false;
      PostQuitMessage(0);
    } break;

    case WM_ACTIVATEAPP: {
      
      OutputDebugStringA("WM_ACTIVATEAPP\n");
    } break;

    case WM_PAINT: {
      PAINTSTRUCT paint;
      HDC device_context = BeginPaint(window, &paint);

      auto x = paint.rcPaint.left;
      auto y = paint.rcPaint.top;
      auto w = paint.rcPaint.right  - paint.rcPaint.left;
      auto h = paint.rcPaint.bottom - paint.rcPaint.top;

      RECT client_rect;
      GetClientRect(window, &client_rect);

      win32_update_window(device_context, &client_rect, x, y, w, h);
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
  win_class.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
  win_class.lpfnWndProc   = MainWindowCallback;
  win_class.hInstance     = hInstance;
  win_class.lpszClassName = "CppWindowClass";

  if (RegisterClass(&win_class)) {
    HWND window_handle = CreateWindowExA(
        0,
        win_class.lpszClassName,
        "Cpp Window",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (window_handle) {
      int offset_x = 0;
      int offset_y = 0;

      running = true;
      while (running) {
        
        MSG message;

        while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
          if (message.message == WM_QUIT) {
            running = false;
          }

          TranslateMessage(&message);
          DispatchMessage(&message);
        }

        render_weird_gradient(offset_x, offset_y);

        HDC device_context = GetDC(window_handle);
        RECT client_rect;
        GetClientRect(window_handle, &client_rect);

        int window_w = client_rect.right - client_rect.left;
        int window_h = client_rect.bottom - client_rect.top;

        win32_update_window(
          device_context,
          &client_rect,
          0, 0, window_w, window_h
        );
        ReleaseDC(window_handle, device_context);

        ++offset_x;
      }
    }
  } else {
    // todo: Logging
  }

  return 0;
}

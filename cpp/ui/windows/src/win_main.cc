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

struct Win32OffscreenBuffer
{
  BITMAPINFO    info;
  void*         memory;
  int           width;
  int           height;
  int           pitch;
  int           byte_per_pixel;
};

struct Win32WindowDimension
{
  int width;
  int height;
};

// TODO: this is a global variable for now.
global_variable bool running;
global_variable Win32OffscreenBuffer global_backbuffer;

Win32WindowDimension
get_window_dimension(HWND window)
{
  Win32WindowDimension result;
  RECT client_rect;
  GetClientRect(window, &client_rect);
  result.width = client_rect.right - client_rect.left;
  result.height = client_rect.bottom - client_rect.top;
  return result;
}

internal void
render_weird_gradient(Win32OffscreenBuffer buffer, int offset_x, int offset_y)
{
  u8 * row = (u8 *)buffer.memory;
  for (int y = 0; y < buffer.height; ++y) {
    u32 * pixel = (u32 *)row;

    for (int x = 0; x < buffer.width; ++x) {
      u8 blue = (x + offset_x);
      u8 green = (y + offset_y);
      /*
        Memory:    BB GG RR xx
        Register:  xx RR GG BB

        Pixel 32-bits
      */
      *pixel++ = ((green << 8) | blue);
    }

    row += buffer.pitch;
  }
}

internal void
win32_resize_dib_section(Win32OffscreenBuffer *buffer, int w, int h)
{
  // TODO: bulletproof this.
  // Maybe don't free first, free after, then free first if that fails.

  if (buffer->memory) {
    VirtualFree(buffer->memory, 0, MEM_RELEASE);
  }

  buffer->width = w;
  buffer->height = h;
  buffer->byte_per_pixel = 4;

  buffer->info.bmiHeader.biSize        = sizeof(buffer->info.bmiHeader);
  buffer->info.bmiHeader.biWidth       = buffer->width;
  buffer->info.bmiHeader.biHeight      = -buffer->height;
  buffer->info.bmiHeader.biPlanes      = 1;
  buffer->info.bmiHeader.biBitCount    = 32;
  buffer->info.bmiHeader.biCompression = BI_RGB;

  int bitmap_memory_size = (buffer->width * buffer->height) * buffer->byte_per_pixel;
  buffer->memory = VirtualAlloc(
    0,
    bitmap_memory_size,
    MEM_COMMIT,
    PAGE_READWRITE
  );

  buffer->pitch = w * buffer->byte_per_pixel;
}

internal void
win32_display_buffer_in_window(HDC device_context, Win32OffscreenBuffer buffer,
                               int window_width, int window_height)
{
  StretchDIBits(device_context,
                0, 0, window_width, window_height,
                0, 0, buffer.width, buffer.height,
                buffer.memory, &buffer.info,
                DIB_RGB_COLORS, SRCCOPY);
}

LRESULT CALLBACK
MainWindowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
  LRESULT result = 0;

  switch (message) {
    case WM_SIZE: {

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

      auto dimension = get_window_dimension(window);

      win32_display_buffer_in_window(
          device_context, global_backbuffer,
          dimension.width, dimension.height);
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
  WNDCLASSA win_class = {};

  win_class.style         = CS_HREDRAW | CS_VREDRAW;
  win_class.lpfnWndProc   = MainWindowCallback;
  win_class.hInstance     = hInstance;
  win_class.lpszClassName = "CppWindowClass";

  win32_resize_dib_section(
    &global_backbuffer,
    1920,
    720
  );

  if (RegisterClassA(&win_class)) {
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

        render_weird_gradient(global_backbuffer, offset_x, offset_y);

        auto device_context = GetDC(window_handle);
        auto dimension = get_window_dimension(window_handle);

        win32_display_buffer_in_window(
          device_context,
          global_backbuffer,
          dimension.width,
          dimension.height
        );

        ReleaseDC(window_handle, device_context);

        ++offset_x;
        offset_y += 2;
      }
    }
  } else {
    // todo: Logging
  }

  return 0;
}

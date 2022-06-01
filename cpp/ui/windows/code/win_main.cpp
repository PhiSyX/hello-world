#include <windows.h>

// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nc-winuser-wndproc
LRESULT CALLBACK
MainWindowCallback(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam)
{
  LRESULT result = 0;
  switch (Message) {
    case WM_SIZE: {
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

      HDC device_context = BeginPaint(Window, &paint);

      auto w = paint.rcPaint.right - paint.rcPaint.left;
      auto h = paint.rcPaint.bottom - paint.rcPaint.top;
      auto x = paint.rcPaint.left;
      auto y = paint.rcPaint.top;

      // https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-patblt
      PatBlt(device_context, x, y, w, h, WHITENESS);

      // https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-endpaint
      EndPaint(Window, &paint);
    } break;
    default: {
      result = DefWindowProc(Window, Message, wParam, lParam);
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
  // https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassa
  WNDCLASS win_class = {};
  // todo: check if HREDRAW/VREDRAW/OWNDC still matter
  win_class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
  win_class.lpfnWndProc = DefWindowProc;
  win_class.hInstance = hInstance;
  win_class.lpszClassName = "CppWindowClass";

  // https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerclassa
  if (RegisterClass(&win_class)) {
    HWND window_handle = CreateWindowExA(0,
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
                                         NULL);

    if (window_handle) {
      MSG message;
      // https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getmessage
      while (GetMessage(&message, NULL, 0, 0) != 0) {
        // https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-translatemessage
        TranslateMessage(&message);
        // https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-dispatchmessage
        DispatchMessage(&message);
      }
    }
  } else {
    // todo: Logging
  }

  return 0;
}

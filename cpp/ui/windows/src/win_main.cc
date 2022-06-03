#include <dsound.h>
#include <stdint.h>
#include <windows.h>
#include <xinput.h>

// @see https://docs.microsoft.com/en-us/windows/win32/api/winuser/
// @see https://docs.microsoft.com/en-us/windows/win32/api/wingdi/
// @see https://docs.microsoft.com/en-us/windows/win32/xinput/
// @see https://docs.microsoft.com/en-us/windows/win32/inputdev/

#define internal static
#define local_persist static
#define global_variable static

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

struct OffscreenBuffer
{
  BITMAPINFO info;
  void* memory;
  int width;
  int height;
  int pitch;
  int byte_per_pixel;
};

struct WindowDimension
{
  int width;
  int height;
};

// TODO: this is a global variable for now.
global_variable bool running;
global_variable OffscreenBuffer global_backbuffer;

// NOTE: XInputGetState
#define X_INPUT_GET_STATE(name)                                                \
  DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE* pState)
typedef X_INPUT_GET_STATE(x_input_get_state);

#define X_INPUT_SET_STATE(name)                                                \
  DWORD WINAPI name(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration)
X_INPUT_GET_STATE(XInputGetStateStub)
{
  return ERROR_DEVICE_NOT_CONNECTED;
}
global_variable x_input_get_state* DynXInputGetState = XInputGetStateStub;
#define XInputGetState DynXInputGetState

// NOTE: XInputSetState
typedef X_INPUT_SET_STATE(x_input_set_state);
X_INPUT_SET_STATE(XInputSetStateStub)
{
  return ERROR_DEVICE_NOT_CONNECTED;
}
global_variable x_input_set_state* DynXInputSetState = XInputSetStateStub;
#define XInputSetState DynXInputSetState

// NOTE: DirectSoundCreate
#define DIRECT_SOUND_CREATE(name)                                              \
  HRESULT WINAPI name(                                                         \
    LPCGUID pcGuidDevice, LPDIRECTSOUND* ppDS, LPUNKNOWN pUnkOuter)
typedef DIRECT_SOUND_CREATE(direct_sound_create);

internal void
win32_load_x_input(void)
{
  HMODULE x_input_library = LoadLibraryA("xinput1_4.dll");

  if (!x_input_library) {
    x_input_library = LoadLibraryA("xinput1_3.dll");
  }

  if (!x_input_library) {
    x_input_library = LoadLibraryA("xinput9_1_0.dll");
  }

  if (!x_input_library) {
    x_input_library = LoadLibraryA("xinput1_3.dll");
  }

  if (!x_input_library) {
    x_input_library = LoadLibraryA("xinput1_2.dll");
  }

  if (!x_input_library) {
    x_input_library = LoadLibraryA("xinput1_1.dll");
  }

  if (!x_input_library) {
    x_input_library = LoadLibraryA("xinput.dll");
  }

  if (x_input_library) {
    XInputGetState =
      (x_input_get_state*)GetProcAddress(x_input_library, "XInputGetState");
    XInputSetState =
      (x_input_set_state*)GetProcAddress(x_input_library, "XInputSetState");
  }
}

internal void
init_d_sound(HWND window, i32 samples_per_sec, i32 buffer_size)
{
  // NOTE: load the library
  HMODULE dsound_library = LoadLibraryA("dsound.dll");

  if (!dsound_library) {
    OutputDebugStringA("dsound.dll not found\n");
    return;
  }

  direct_sound_create* DirectSoundCreate =
    (direct_sound_create*)GetProcAddress(dsound_library, "DirectSoundCreate");
  if (!DirectSoundCreate) {
    OutputDebugStringA("DirectSoundCreate not found\n");
    return;
  }

  // NOTE: get a DirectSound object! (coop)

  LPDIRECTSOUND direct_sound;
  DirectSoundCreate(0, &direct_sound, 0);
  direct_sound->SetCooperativeLevel(window, DSSCL_PRIORITY);

  // NOTE: create a primary sound buffer
  DSBUFFERDESC sound_buffer_desc_p = {};
  sound_buffer_desc_p.dwSize = sizeof(sound_buffer_desc_p);
  sound_buffer_desc_p.dwFlags = DSBCAPS_PRIMARYBUFFER;

  LPDIRECTSOUNDBUFFER primary_buffer;
  direct_sound->CreateSoundBuffer(&sound_buffer_desc_p, &primary_buffer, 0);

  WAVEFORMATEX wave_format = {};
  wave_format.wFormatTag = WAVE_FORMAT_PCM;
  wave_format.nChannels = 2;
  wave_format.nSamplesPerSec = samples_per_sec;
  wave_format.wBitsPerSample = 16;
  wave_format.nBlockAlign =
    (wave_format.nChannels * wave_format.wBitsPerSample) / 8;
  wave_format.nAvgBytesPerSec =
    wave_format.nSamplesPerSec * wave_format.nBlockAlign;
  wave_format.cbSize = 0;
  primary_buffer->SetFormat(&wave_format);

  // NOTE: create a secondary sound buffer
  DSBUFFERDESC sound_buffer_desc_s = {};
  sound_buffer_desc_s.dwSize = sizeof(sound_buffer_desc_s);
  sound_buffer_desc_s.dwFlags = DSBCAPS_PRIMARYBUFFER;
  sound_buffer_desc_s.dwBufferBytes = buffer_size;
  sound_buffer_desc_s.lpwfxFormat = &wave_format;

  LPDIRECTSOUNDBUFFER secondary_buffer;

  direct_sound->CreateSoundBuffer(&sound_buffer_desc_s, &secondary_buffer, 0);

  // NOTE: start it playing!
}

internal WindowDimension
get_window_dimension(HWND window)
{
  WindowDimension result;
  RECT client_rect;
  GetClientRect(window, &client_rect);
  result.width = client_rect.right - client_rect.left;
  result.height = client_rect.bottom - client_rect.top;
  return result;
}

internal void
render_weird_gradient(OffscreenBuffer* buffer, int offset_x, int offset_y)
{
  u8* row = (u8*)buffer->memory;
  for (int y = 0; y < buffer->height; ++y) {
    u32* pixel = (u32*)row;

    for (int x = 0; x < buffer->width; ++x) {
      u8 blue = (x + offset_x);
      u8 green = (y + offset_y);
      /*
        Memory:    BB GG RR xx
        Register:  xx RR GG BB

        Pixel 32-bits
      */
      *pixel++ = ((green << 8) | blue);
    }

    row += buffer->pitch;
  }
}

internal void
resize_dib_section(OffscreenBuffer* buffer, int w, int h)
{
  // TODO: bulletproof this.
  // Maybe don't free first, free after, then free first if that fails.

  if (buffer->memory) {
    VirtualFree(buffer->memory, 0, MEM_RELEASE);
  }

  buffer->width = w;
  buffer->height = h;
  buffer->byte_per_pixel = 4;

  buffer->info.bmiHeader.biSize = sizeof(buffer->info.bmiHeader);
  buffer->info.bmiHeader.biWidth = buffer->width;
  buffer->info.bmiHeader.biHeight = -buffer->height;
  buffer->info.bmiHeader.biPlanes = 1;
  buffer->info.bmiHeader.biBitCount = 32;
  buffer->info.bmiHeader.biCompression = BI_RGB;

  int bitmap_memory_size =
    (buffer->width * buffer->height) * buffer->byte_per_pixel;
  buffer->memory = VirtualAlloc(
    0, bitmap_memory_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

  buffer->pitch = w * buffer->byte_per_pixel;
}

internal void
display_buffer_in_window(HDC device_context,
                         OffscreenBuffer* buffer,
                         int window_width,
                         int window_height)
{
  StretchDIBits(device_context,
                0,
                0,
                window_width,
                window_height,
                0,
                0,
                buffer->width,
                buffer->height,
                buffer->memory,
                &buffer->info,
                DIB_RGB_COLORS,
                SRCCOPY);
}

LRESULT CALLBACK
MainWindowCallback(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
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

    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_KEYDOWN:
    case WM_KEYUP: {
      u32 vk_code = w_param;

      bool alt_key_was_down = (l_param & (1 << 29));
      bool was_down = ((l_param & (1 << 30)) != 0);
      bool is_down = ((l_param & (1 << 31)) == 0);

      if (was_down == is_down) {
        break;
      }

      if (vk_code == 'Z') {
      } else if (vk_code == 'Q') {
      } else if (vk_code == 'S') {
      } else if (vk_code == 'D') {
      } else if (vk_code == VK_UP) {
      } else if (vk_code == VK_DOWN) {
      } else if (vk_code == VK_LEFT) {
      } else if (vk_code == VK_RIGHT) {
      } else if (vk_code == VK_ESCAPE) {
      } else if (vk_code == VK_SPACE) {
      }

      if (vk_code == VK_F4 && alt_key_was_down) {
        running = false;
      }
    } break;

    case WM_PAINT: {
      PAINTSTRUCT paint;
      HDC device_context = BeginPaint(window, &paint);

      auto dimension = get_window_dimension(window);

      display_buffer_in_window(
        device_context, &global_backbuffer, dimension.width, dimension.height);
      EndPaint(window, &paint);
    } break;

    default: {
      result = DefWindowProc(window, message, w_param, l_param);
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

  win_class.style = CS_HREDRAW | CS_VREDRAW;
  win_class.lpfnWndProc = MainWindowCallback;
  win_class.hInstance = hInstance;
  win_class.lpszClassName = "CppWindowClass";

  resize_dib_section(&global_backbuffer, 1920, 720);

  if (RegisterClassA(&win_class)) {
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
      int offset_x = 0;
      int offset_y = 0;

      init_d_sound(window_handle, 48000, 48000 * sizeof(i16) * 2);

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

        for (DWORD controller_idx = 0; controller_idx < XUSER_MAX_COUNT;
             ++controller_idx) {
          XINPUT_STATE controller_state;
          if (XInputGetState(controller_idx, &controller_state) ==
              ERROR_SUCCESS) {
            // NOTE: this controller is plugged in
            // TODO: see if controller_state.dwPacketNumber increments too
            // rapidly
            XINPUT_GAMEPAD* pad = &controller_state.Gamepad;

            bool pad_up = (pad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
            bool pad_down = (pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
            bool pad_left = (pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
            bool pad_right = (pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
            bool pad_start = (pad->wButtons & XINPUT_GAMEPAD_START);
            bool pad_back = (pad->wButtons & XINPUT_GAMEPAD_BACK);
            bool pad_left_shoulder =
              (pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
            bool pad_right_shoulder =
              (pad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
            bool pad_a = (pad->wButtons & XINPUT_GAMEPAD_A);
            bool pad_b = (pad->wButtons & XINPUT_GAMEPAD_B);
            bool pad_x = (pad->wButtons & XINPUT_GAMEPAD_X);
            bool pad_y = (pad->wButtons & XINPUT_GAMEPAD_Y);

            i16 pad_left_stick_x = pad->sThumbLX;
            i16 pad_left_stick_y = pad->sThumbLY;

            offset_x += pad_left_stick_x >> 12;
            offset_y += pad_left_stick_y >> 12;
          } else {
            // NOTE: this controller is not available
          }
        }

        render_weird_gradient(&global_backbuffer, offset_x, offset_y);

        auto device_context = GetDC(window_handle);
        auto dimension = get_window_dimension(window_handle);

        display_buffer_in_window(device_context,
                                 &global_backbuffer,
                                 dimension.width,
                                 dimension.height);

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

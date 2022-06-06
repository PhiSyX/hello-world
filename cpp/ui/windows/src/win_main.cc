#include <atlstr.h>
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
typedef uint32_t usize;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef int32_t isize;

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
global_variable LPDIRECTSOUNDBUFFER global_sound_buffer;

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
init_d_sound(HWND window, i32 buffer_size, i32 samples_per_sec)
{
  // NOTE: load the library
  HMODULE dsound_library = LoadLibraryA("dsound.dll");

  if (!dsound_library) {
    MessageBoxA(window, "dsound.dll not found", 0, 0);
    return;
  }

  direct_sound_create* DirectSoundCreate =
    (direct_sound_create*)GetProcAddress(dsound_library, "DirectSoundCreate");
  if (!DirectSoundCreate) {
    MessageBoxA(window, "DirectSoundCreate not found", "Error", MB_OK);
    return;
  }

  // NOTE: get a DirectSound object! (coop)

  LPDIRECTSOUND direct_sound;
  if (!SUCCEEDED(DirectSoundCreate(0, &direct_sound, 0))) {
    MessageBoxA(window, "DirectSoundCreate failed", 0, 0);
    return;
  }

  if (!SUCCEEDED(direct_sound->SetCooperativeLevel(window, DSSCL_PRIORITY))) {
    MessageBoxA(window, "SetCooperativeLevel failed", 0, 0);
    return;
  }

  if (DirectSoundCreate && SUCCEEDED(DirectSoundCreate(0, &direct_sound, 0))) {
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

    if (SUCCEEDED(direct_sound->SetCooperativeLevel(window, DSSCL_PRIORITY))) {
      DSBUFFERDESC buffer_description = {};
      buffer_description.dwSize = sizeof(buffer_description);
      buffer_description.dwFlags = DSBCAPS_PRIMARYBUFFER;

      // NOTE: "Create" a primary buffer
      // TODO: DSBCAPS_GLOBALFOCUS?
      LPDIRECTSOUNDBUFFER primary_buffer;
      if (SUCCEEDED(direct_sound->CreateSoundBuffer(
            &buffer_description, &primary_buffer, 0))) {
        HRESULT error = primary_buffer->SetFormat(&wave_format);
        if (!SUCCEEDED(error)) {
          MessageBoxA(window, "SetFormat failed", 0, 0);
          return;
        }
      } else {
        // TODO: Diagnostic
      }
    } else {
      // TODO: Diagnostic
    }

    // TODO: DSBCAPS_GETCURRENTPOSITION2
    DSBUFFERDESC buffer_description = {};
    buffer_description.dwSize = sizeof(buffer_description);
    buffer_description.dwFlags = 0;
    buffer_description.dwBufferBytes = buffer_size;
    buffer_description.lpwfxFormat = &wave_format;

    HRESULT error = direct_sound->CreateSoundBuffer(
      &buffer_description, &global_sound_buffer, 0);
    if (!SUCCEEDED(error)) {
      MessageBoxA(window, "Secondary buffer created", 0, 0);
    }
  } else {
    // TODO: Diagnostic
  }
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

  auto atom = RegisterClassA(&win_class);
  if (atom == 0) {
    return 1;
  }

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
  if (!window_handle) {
    return 0;
  }

  // NOTE: since we specified CS_OWNDC, we can just get one device context
  // and use it forever because we are not sharing it with anyone.
  auto device_context = GetDC(window_handle);

  // NOTE: Graphics
  i32 offset_x = 0;
  i32 offset_y = 0;

  // NOTE: Sound
  int samples_per_second = 48000;
  i16 tone_hz = 256;
  i16 tone_volume = 800;
  usize running_sample_idx = 0;
  int square_wave_period = samples_per_second / tone_hz;
  int half_square_wave_period = square_wave_period / 2;
  int bytes_per_sample = sizeof(i16) * 2;
  int secondary_buffer_size = samples_per_second * bytes_per_sample;

  init_d_sound(window_handle, secondary_buffer_size, samples_per_second);
  global_sound_buffer->Play(0, 0, DSBPLAY_LOOPING);

  running = true;
  while (running) {
    MSG message;

    while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
      if (message.message == WM_QUIT) {
        running = false;
      }

      TranslateMessage(&message);
      DispatchMessage(&message);
    }

    // TODO: should we poll this more frequently
    for (DWORD controller_idx = 0; controller_idx < XUSER_MAX_COUNT;
         ++controller_idx) {
      XINPUT_STATE controller_state;
      if (XInputGetState(controller_idx, &controller_state) == ERROR_SUCCESS) {
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
        bool pad_left_shoulder = (pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
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

    // NOTE: DirectSound output
    DWORD play_cursor;
    DWORD write_cursor;
    if (SUCCEEDED(global_sound_buffer->GetCurrentPosition(&play_cursor,
                                                          &write_cursor))) {
      DWORD byte_to_lock =
        running_sample_idx * bytes_per_sample % secondary_buffer_size;

      DWORD byte_to_write;
      if (byte_to_lock > play_cursor) {
        byte_to_write = (secondary_buffer_size - byte_to_lock);
        byte_to_write += play_cursor;
      } else {
        byte_to_write = play_cursor - byte_to_lock;
      }

      VOID* region_1;
      DWORD region_1_size;
      VOID* region_2;
      DWORD region_2_size;

      if (SUCCEEDED(global_sound_buffer->Lock(byte_to_lock,
                                              byte_to_write,
                                              &region_1,
                                              &region_1_size,
                                              &region_2,
                                              &region_2_size,
                                              0))) {
        // TODO: assert that region_1_size and region_2_size is valid
        i16* sample_out = (i16*)region_1;
        DWORD region1_sample_count = region_1_size / bytes_per_sample;
        DWORD region_2_sample_count = region_2_size / bytes_per_sample;
        for (DWORD idx = 0; idx < region1_sample_count; ++idx) {
          i16 sample_value =
            ((running_sample_idx++ / half_square_wave_period) % 2)
              ? tone_volume
              : -tone_volume;
          *sample_out++ = sample_value;
          *sample_out++ = sample_value;
        }
        for (DWORD idx = 0; idx < region_2_sample_count; ++idx) {
          i16 sample_value =
            ((running_sample_idx++ / half_square_wave_period) % 2)
              ? tone_volume
              : -tone_volume;
          *sample_out++ = sample_value;
          *sample_out++ = sample_value;
        }

        // Observe "clicking" effect if you _dont_ unlock
        global_sound_buffer->Unlock(
          region_1, region_1_size, region_2, region_2_size);
      }
    }

    auto dimension = get_window_dimension(window_handle);

    display_buffer_in_window(
      device_context, &global_backbuffer, dimension.width, dimension.height);

    ++offset_x;
    offset_y += 2;
  }

  return 0;
}

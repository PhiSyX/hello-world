// Lancer en mode release, car évidement plus fluide
// @see https://docs.microsoft.com/en-us/windows/win32/api/wingdi/
// @see https://docs.microsoft.com/en-us/windows/win32/api/wingdi/
// @see https://docs.microsoft.com/en-us/windows/win32/xinput/
// @see https://docs.microsoft.com/en-us/windows/win32/inputdev/

use std::{
    mem,
    sync::atomic::{AtomicPtr, Ordering},
};

use windows::Win32::{
    Foundation::*,
    Graphics::Gdi::*,
    System::{LibraryLoader::*, Memory::*},
    UI::{Input::XboxController::*, WindowsAndMessaging::*},
};
use windows_sys::Win32::UI::Input::KeyboardAndMouse::*;

struct AppState {
    running: bool,
    backbuffer: OffscreenBuffer,
}

struct OffscreenBuffer {
    info: BITMAPINFO,
    memory: *mut core::ffi::c_void,
    width: i32,
    height: i32,
    pitch: i32,
    byte_per_pixel: i32,
}

struct WindowDimension {
    width: i32,
    height: i32,
}

lazy_static::lazy_static! {
    static ref GLOBAL_APP_STATE: AtomicPtr<AppState> =
        AtomicPtr::new(Box::into_raw(Box::new(AppState::default())));
}

fn main() {
    unsafe {
        let instance = GetModuleHandleA(None).expect("L'instance");

        let win_class = WNDCLASSA {
            style: CS_HREDRAW | CS_VREDRAW,
            lpfnWndProc: Some(main_window_callback),
            hInstance: instance,
            lpszClassName: windows::core::PCSTR(
                b"RustWindowClass" as *const u8,
            ),
            ..Default::default()
        };

        let global_app_state =
            &mut *GLOBAL_APP_STATE.load(Ordering::SeqCst);

        win32_resize_dib_section(
            &mut global_app_state.backbuffer,
            1280,
            720,
        );

        let atom = RegisterClassA(&win_class);
        assert!(atom != 0);

        let window_handle = CreateWindowExA(
            Default::default(),
            win_class.lpszClassName,
            "Rust Window",
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            None,
            None,
            instance,
            std::ptr::null(),
        );

        let mut offset_x: i32 = 0;
        let mut offset_y: i32 = 0;

        global_app_state.running = true;

        while global_app_state.running {
            let mut msg = MSG::default();

            while PeekMessageA(&mut msg, None, 0, 0, PM_REMOVE) != false {
                if msg.message == WM_QUIT {
                    global_app_state.running = false;
                }

                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }

            for controller_idx in 0..XUSER_MAX_COUNT {
                let mut state = XINPUT_STATE::default();
                if XInputGetState(controller_idx, &mut state) == 0 {
                    // NOTE: this controller is plugged in
                    // TODO: see if controller_state.dwPacketNumber
                    // increments too rapidly
                    let pad = &state.Gamepad;

                    let pad_left_stick_x = pad.sThumbLX;
                    let pad_left_stick_y = pad.sThumbLY;

                    offset_x += (pad_left_stick_x >> 12) as i32;
                    offset_y += (pad_left_stick_y >> 12) as i32;
                }
            }

            render_weird_gradient(
                &mut global_app_state.backbuffer,
                offset_x,
                offset_y,
            );

            let device_context: HDC = GetDC(window_handle);
            let dimension = WindowDimension::new(window_handle);

            win32_display_buffer_in_window(
                device_context,
                dimension,
                &global_app_state.backbuffer,
            );
            ReleaseDC(window_handle, device_context);

            offset_x += 1;
            offset_y += 1;
        }
    }
}

unsafe extern "system" fn main_window_callback(
    window: HWND,
    message: u32,
    w_param: WPARAM,
    l_param: LPARAM,
) -> LRESULT {
    match message {
        | WM_SIZE => {
            println!("WM_SIZE");
            Default::default()
        }

        | WM_DESTROY => {
            println!("WM_DESTROY");
            let global_app_state =
                &mut *GLOBAL_APP_STATE.load(Ordering::SeqCst);
            global_app_state.running = false;
            PostQuitMessage(0);
            Default::default()
        }

        | WM_CLOSE => {
            println!("WM_CLOSE");
            let global_app_state =
                &mut *GLOBAL_APP_STATE.load(Ordering::SeqCst);
            global_app_state.running = false;
            PostQuitMessage(0);
            Default::default()
        }

        | WM_ACTIVATEAPP => {
            println!("WM_ACTIVATEAPP");
            Default::default()
        }

        | WM_SYSKEYDOWN | WM_SYSKEYUP | WM_KEYDOWN | WM_KEYUP => {
            let vk_code = w_param.0 as u8 as char;
            let was_down = (l_param.0 & (1 << 30)) != 0;
            let is_down = (l_param.0 & (1 << 31)) == 0;

            if was_down == is_down {
                return Default::default();
            }

            match vk_code {
                | 'Z' => {
                    println!("Z");
                }
                | 'Q' => {
                    println!("Q");
                }
                | 'S' => {
                    println!("S");
                }
                | 'D' => {
                    println!("D");
                }

                // NOTE: for the example
                | ch if ch == VK_ESCAPE as u8 as char => {
                    if was_down {
                        println!("ESCAPE");

                        let global_app_state =
                            &mut *GLOBAL_APP_STATE.load(Ordering::SeqCst);

                        global_app_state.running = false;
                    }
                }
                | _ => {}
            }

            Default::default()
        }

        | WM_PAINT => {
            println!("WM_PAINT");

            let mut paint = Default::default();

            let device_context = BeginPaint(window, &mut paint);

            let global_app_state =
                &mut *GLOBAL_APP_STATE.load(Ordering::SeqCst);

            let dimension = WindowDimension::new(window);
            win32_display_buffer_in_window(
                device_context,
                dimension,
                &global_app_state.backbuffer,
            );

            EndPaint(window, &paint);
            Default::default()
        }

        | _ => DefWindowProcA(window, message, w_param, l_param),
    }
}

fn win32_resize_dib_section(
    backbuffer: &mut OffscreenBuffer,
    width: i32,
    height: i32,
) {
    // TODO: bulletproof this.
    // Maybe don't free first, free after, then free first if that
    // fails.

    if !backbuffer.memory.is_null() {
        unsafe { VirtualFree(backbuffer.memory, 0, MEM_RELEASE) };
    }

    backbuffer.width = width;
    backbuffer.height = height;
    backbuffer.byte_per_pixel = 4;

    let bitmap_width = backbuffer.width;
    let bitmap_height = backbuffer.height;

    backbuffer.info.bmiHeader.biSize =
        mem::size_of_val(&backbuffer.info.bmiHeader) as u32;
    backbuffer.info.bmiHeader.biWidth = bitmap_width;
    backbuffer.info.bmiHeader.biHeight = -bitmap_height;

    backbuffer.pitch = width * backbuffer.byte_per_pixel;

    let bitmap_memory_size =
        (bitmap_width * bitmap_height) * backbuffer.byte_per_pixel as i32;

    backbuffer.memory = unsafe {
        VirtualAlloc(
            core::ptr::null_mut(),
            bitmap_memory_size as usize,
            MEM_COMMIT,
            PAGE_READWRITE,
        )
    };
}

unsafe fn win32_display_buffer_in_window(
    device_context: HDC,
    dimension: WindowDimension,
    backbuffer: &OffscreenBuffer,
) {
    // TODO: aspect ratio correction.
    StretchDIBits(
        device_context,
        0,
        0,
        dimension.width,
        dimension.height,
        0,
        0,
        backbuffer.width,
        backbuffer.height,
        backbuffer.memory,
        &backbuffer.info,
        DIB_RGB_COLORS,
        SRCCOPY,
    );
}

unsafe fn render_weird_gradient(
    backbuffer: &mut OffscreenBuffer,
    offset_x: i32,
    offset_y: i32,
) {
    let mut row = backbuffer.memory;

    for y in 0..backbuffer.height {
        let mut pixel = row as *mut [u8; 4];

        for x in 0..backbuffer.width {
            let blue = x + offset_x;
            let green = y + offset_y;
            *pixel = [blue as u8, green as u8, 0, 0];
            pixel = pixel.offset(1);
        }

        row = row.offset(backbuffer.pitch as isize);
    }
}

// -------------- //
// Implémentation //
// -------------- //

impl AppState {
    const fn default() -> Self {
        Self {
            running: true,
            backbuffer: OffscreenBuffer::default(),
        }
    }
}

impl OffscreenBuffer {
    const fn default() -> Self {
        Self {
            info: BITMAPINFO {
                bmiHeader: BITMAPINFOHEADER {
                    biSize: 0,
                    biWidth: 0,
                    biHeight: 0,
                    biPlanes: 1,
                    biBitCount: 32,
                    biCompression: BI_RGB as u32,
                    biSizeImage: 0,
                    biXPelsPerMeter: 0,
                    biYPelsPerMeter: 0,
                    biClrUsed: 0,
                    biClrImportant: 0,
                },
                bmiColors: [RGBQUAD {
                    rgbBlue: 0,
                    rgbGreen: 0,
                    rgbRed: 0,
                    rgbReserved: 0,
                }; 1],
            },
            memory: std::ptr::null_mut(),
            width: 0,
            height: 0,
            pitch: 0,
            byte_per_pixel: 4,
        }
    }
}

impl WindowDimension {
    fn new(window: HWND) -> Self {
        let mut client_rect: RECT = Default::default();
        unsafe { GetClientRect(window, &mut client_rect) };
        let width = client_rect.right - client_rect.left;
        let height = client_rect.bottom - client_rect.top;
        Self { width, height }
    }
}

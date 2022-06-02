use core::slice;
use std::{mem, ops::AddAssign, sync::Mutex};

use lazy_static::lazy_static;
use windows::Win32::{
    Foundation::*,
    Graphics::Gdi::*,
    System::{LibraryLoader::*, Memory::*},
    UI::WindowsAndMessaging::*,
};

// @see https://docs.microsoft.com/en-us/windows/win32/api/winuser/
// @see https://docs.microsoft.com/en-us/windows/win32/api/wingdi/

#[derive(Default)]
struct AppState {
    running: bool,
    bitmap: Bitmap,
}

struct Bitmap {
    info: BITMAPINFO,
    memory: *mut core::ffi::c_void,
    width: i32,
    height: i32,
}

lazy_static! {
    static ref APP_STATE: Mutex<AppState> =
        Mutex::new(AppState::default());
}
const BYTE_PER_PIXEL: u8 = 4;

fn main() {
    let instance = unsafe { GetModuleHandleA(None) }.expect("L'instance");

    let win_class = WNDCLASSA {
        style: CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
        lpfnWndProc: Some(main_window_callback),
        hInstance: instance,
        lpszClassName: windows::core::PCSTR(
            b"RustWindowClass" as *const u8,
        ),
        ..Default::default()
    };

    unsafe {
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

        let mut offset_x = 0;
        let mut offset_y = 0;

        APP_STATE.lock().unwrap().running = true;

        while APP_STATE.lock().unwrap().running {
            let mut msg = MSG::default();

            while PeekMessageA(&mut msg, None, 0, 0, PM_REMOVE) != false {
                if msg.message == WM_QUIT {
                    APP_STATE.lock().unwrap().running = false;
                }

                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }

            render_weird_gradient(offset_x, offset_y);

            let device_context: HDC = GetDC(window_handle);
            let mut client_rect: RECT = Default::default();
            GetClientRect(window_handle, &mut client_rect);

            let window_width = client_rect.right - client_rect.left;
            let window_height = client_rect.bottom - client_rect.top;

            win32_update_window(
                device_context,
                &client_rect,
                0,
                0,
                window_width,
                window_height,
            );
            ReleaseDC(window_handle, device_context);

            offset_x += 1;
            offset_y += 2;
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
            let mut client_rect: RECT = Default::default();
            GetClientRect(window, &mut client_rect);

            let w = client_rect.right - client_rect.left;
            let h = client_rect.bottom - client_rect.top;

            win32_resize_dib_section(w, h);

            Default::default()
        }
        | WM_DESTROY => {
            println!("WM_DESTROY");
            APP_STATE.lock().unwrap().running = false;
            PostQuitMessage(0);
            Default::default()
        }
        | WM_CLOSE => {
            println!("WM_CLOSE");
            APP_STATE.lock().unwrap().running = false;
            PostQuitMessage(0);
            Default::default()
        }
        | WM_ACTIVATEAPP => {
            println!("WM_ACTIVATEAPP");
            Default::default()
        }
        | WM_PAINT => {
            println!("WM_PAINT");

            let mut paint = Default::default();

            let device_context = BeginPaint(window, &mut paint);

            let x = paint.rcPaint.left;
            let y = paint.rcPaint.top;
            let w = paint.rcPaint.right - x;
            let h = paint.rcPaint.bottom - y;

            let mut client_rect: RECT = Default::default();
            GetClientRect(window, &mut client_rect);
            win32_update_window(device_context, &client_rect, x, y, w, h);

            EndPaint(window, &paint);
            Default::default()
        }

        | _ => DefWindowProcA(window, message, w_param, l_param),
    }
}

fn win32_resize_dib_section(width: i32, height: i32) {
    // TODO: bulletproof this.
    // Maybe don't free first, free after, then free first if that fails.

    if !APP_STATE.lock().unwrap().bitmap.memory.is_null() {
        unsafe {
            VirtualFree(
                APP_STATE.lock().unwrap().bitmap.memory,
                0,
                MEM_RELEASE,
            )
        };
    }

    let mut app_state = APP_STATE.lock().unwrap();

    app_state.bitmap.width = width;
    app_state.bitmap.height = height;

    let bitmap_width = app_state.bitmap.width;
    let bitmap_height = app_state.bitmap.height;

    app_state.bitmap.info.bmiHeader.biSize =
        mem::size_of_val(&app_state.bitmap.info.bmiHeader) as u32;
    app_state.bitmap.info.bmiHeader.biWidth = bitmap_width;
    app_state.bitmap.info.bmiHeader.biHeight = -bitmap_height;
    app_state.bitmap.info.bmiHeader.biPlanes = 1;
    app_state.bitmap.info.bmiHeader.biBitCount = 32;
    app_state.bitmap.info.bmiHeader.biCompression = BI_RGB as u32;

    let bitmap_memory_size =
        bitmap_width * bitmap_height * BYTE_PER_PIXEL as i32;

    app_state.bitmap.memory = unsafe {
        VirtualAlloc(
            core::ptr::null_mut(),
            bitmap_memory_size as usize,
            MEM_COMMIT,
            PAGE_READWRITE,
        )
    };
}

fn win32_update_window(
    device_context: HDC,
    client_rect: &RECT,
    x: i32,
    y: i32,
    w: i32,
    h: i32,
) {
    let bitmap = &APP_STATE.lock().unwrap().bitmap;
    let window_width = client_rect.right - client_rect.left;
    let window_height = client_rect.bottom - client_rect.top;
    unsafe {
        StretchDIBits(
            device_context,
            0,
            0,
            bitmap.width,
            bitmap.height,
            0,
            0,
            window_width,
            window_height,
            bitmap.memory,
            &bitmap.info,
            DIB_RGB_COLORS,
            SRCCOPY,
        )
    };
}

unsafe fn render_weird_gradient(offset_x: i32, offset_y: i32) {
    let state = APP_STATE.lock().unwrap();

    let width = state.bitmap.width;
    let height = state.bitmap.height;
    let size = ((width - BYTE_PER_PIXEL as i32) * height) as usize;

    slice::from_raw_parts_mut(state.bitmap.memory as *mut u32, size)
        .chunks_mut(width as usize)
        .enumerate()
        .for_each(|(x, row)| {
            let blue = offset_x.wrapping_add(x as i32);
            row.iter_mut().enumerate().for_each(|(y, pixel)| {
                let green = offset_y.wrapping_add(y as i32);
                *pixel = ((blue << 8) | green) as u32;
            });
        });
}

// -------------- //
// Implémentation //
// -------------- //

unsafe impl Send for AppState {}
unsafe impl Sync for AppState {}

impl Default for Bitmap {
    fn default() -> Self {
        Self {
            info: Default::default(),
            memory: std::ptr::null_mut(),
            width: Default::default(),
            height: Default::default(),
        }
    }
}

use windows::Win32::{
    Foundation::*, Graphics::Gdi::*, System::LibraryLoader::*,
    UI::WindowsAndMessaging::*,
};

fn main() {
    let instance = unsafe { GetModuleHandleA(None) }.expect("L'instance");

    // https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassa
    let win_class = WNDCLASSA {
        style: CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
        lpfnWndProc: Some(main_window_callback),
        hInstance: instance,
        lpszClassName: windows::core::PCSTR(
            b"RustWindowClass" as *const u8,
        ),
        ..Default::default()
    };

    // https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerclassa
    unsafe {
        let atom = RegisterClassA(&win_class);
        assert!(atom != 0);

        CreateWindowExA(
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

        let mut message: MSG = Default::default();
        // https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getmessagea
        while GetMessageA(&mut message, None, 0, 0) != false {
            // https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-translatemessage
            TranslateMessage(&message);
            // https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-dispatchmessagea
            DispatchMessageA(&message);
        }
    }
}

// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nc-winuser-wndproc
unsafe extern "system" fn main_window_callback(
    window: HWND,
    message: u32,
    wParam: WPARAM,
    lParam: LPARAM,
) -> LRESULT {
    match message {
        | WM_SIZE => {
            println!("WM_SIZE");
            Default::default()
        }
        | WM_DESTROY => {
            println!("WM_DESTROY");
            PostQuitMessage(0);
            Default::default()
        }
        | WM_CLOSE => {
            println!("WM_CLOSE");
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

            // https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-beginpaint
            let device_context = BeginPaint(window, &mut paint);

            let w = paint.rcPaint.right - paint.rcPaint.left;
            let h = paint.rcPaint.bottom - paint.rcPaint.top;
            let x = paint.rcPaint.left;
            let y = paint.rcPaint.top;

            // https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-patblt
            PatBlt(device_context, x, y, w, h, WHITENESS);

            // https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-endpaint
            EndPaint(window, &paint);

            Default::default()
        }

        | _ => DefWindowProcA(window, message, wParam, lParam),
    }
}

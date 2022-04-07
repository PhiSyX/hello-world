use std::{
    cell::{RefCell, RefMut},
    rc::Rc,
    sync::{Arc, Mutex},
    thread, time,
};

pub fn run() {
    rc();
    arc();
}

fn rc() {
    let ptr = Rc::new(42);

    {
        let ptr2 = Rc::clone(&ptr);
        println!("{}", *ptr2);
    }

    println!("{}", *ptr);

    let shared_string = Rc::new(RefCell::new("John".into()));

    {
        let mut doe: RefMut<String> = shared_string.borrow_mut();
        doe.push_str(" Doe");
    }

    println!("{}", shared_string.take());
}

fn arc() {
    let ptr = Arc::new(42);
    let ptr2 = Arc::clone(&ptr);

    thread::spawn(move || {
        println!("{}", *ptr2);
    });

    thread::sleep(time::Duration::from_secs(1));
    println!("{}", *ptr);

    arc_mutex();
}

fn arc_mutex() {
    let ptr = Arc::new(Mutex::new(42));
    let ptr2 = Arc::clone(&ptr);

    thread::spawn(move || {
        let mut mut_ptr = ptr2.lock().unwrap();
        *mut_ptr = 19;
    });

    thread::sleep(time::Duration::from_secs(1));
    let res = ptr.lock().unwrap();
    println!("{res}");
}

use std::rc::Rc;

// Modélisation grossière d'un unique_ptr<int> car les pointeurs C++
// peuvent toujours être `nullptr`.
pub fn use_unique_ptr_segfault() {
    fn seg_fault() {
        panic!("Segmentation fault");
    }

    let mut orig: Option<Box<u8>> = Some(box 5);

    match orig {
        None => seg_fault(),

        Some(orig_nonnull) => {
            println!("{}", *orig_nonnull);

            // L'équivalent de unique_tr move de C++.
            let stolen = Some(orig_nonnull);
            orig = None;

            match orig {
                None => seg_fault(),
                Some(orig_nonnull) => println!("{}", *orig_nonnull),
            }
        }
    }
}

pub fn use_unique_ptr_smart_pointer() {
    let v = vec![Rc::new(5u8)];

    println!("{}", *v[0]);

    let pointer_to_5 = v[0].clone();
    println!("{}", *pointer_to_5);

    println!("{}", *v[0]);
}

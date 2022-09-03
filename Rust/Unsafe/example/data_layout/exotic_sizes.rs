#[derive(Debug)]
pub struct MySuperSliceable<T: ?Sized> {
    pub info: u32,
    pub data: T,
}

pub fn exotic_sizes() {
    let sized: MySuperSliceable<[u8; 8]> = MySuperSliceable {
        info: 17,
        data: [0; 8],
    };

    let dynamic: &MySuperSliceable<[u8]> = &sized;

    dbg!(dynamic);
}

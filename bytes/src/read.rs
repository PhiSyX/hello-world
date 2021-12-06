use std::{fmt::Display, fs::File, io};

// --------- //
// Structure //
// --------- //

#[derive(Debug)]
pub(super) struct BytesRead {
    bytes: Vec<u8>,
    len: usize,
}

// -------------- //
// Implémentation //
// -------------- //

impl BytesRead {
    pub(super) fn new(bytes: Vec<u8>, len: usize) -> Self {
        Self { bytes, len }
    }
}

// -------------- //
// Implémentation // -> Interface
// -------------- //

impl Display for BytesRead {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{} bytes read:\n{:02x?}", self.len, self.bytes)
    }
}

// --- //
// App //
// --- //

pub(crate) fn run() -> io::Result<()> {
    let mut f = File::open("Cargo.toml")?;
    let read_bytes = read_bytes::<_, 1024>(&mut f)?;
    println!("{}", read_bytes);
    Ok(())
}

fn read_bytes<R: io::Read, const N: usize>(
    f: &mut R,
) -> io::Result<BytesRead> {
    let mut buf = [0u8; N];
    let len = f.read(&mut buf)?;
    Ok(BytesRead::new(buf.into(), len))
}

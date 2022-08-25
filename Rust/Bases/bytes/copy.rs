use std::io;

pub(crate) fn run() -> io::Result<()> {
    let mut read: &[u8] = &b"abcdefg".repeat(500 * 1024); // 358.4KB
    let output = copy_bytes(&mut read)?;
    println!("{} bytes read", output);

    Ok(())
}

fn copy_bytes<R: ?Sized + io::Read>(reader: &mut R) -> io::Result<usize> {
    const BUFFER_SIZE: usize = 8 * 1024;
    let mut dst: Vec<u8> = vec![];
    let mut buf: [u8; BUFFER_SIZE] = [0; BUFFER_SIZE]; // 8KB Buffer
    copy_buf(reader, &mut dst, &mut buf)
}

fn copy_buf<R: ?Sized + io::Read, W: ?Sized + io::Write>(
    r: &mut R,
    w: &mut W,
    b: &mut [u8],
) -> io::Result<usize> {
    let mut wsize: usize;
    let mut written: usize = 0;

    loop {
        match r.read(b)? {
            | 0 => break,
            | rsize => {
                wsize = w.write(&b[..rsize])?;
                written += wsize;
            }
        }
    }

    w.flush()?;

    Ok(written)
}

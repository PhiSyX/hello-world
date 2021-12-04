use std::{fmt, fmt::Display};

use rand::RngCore;

// --------- //
// Structure //
// --------- //

#[derive(Debug)]
struct MacAddr([u8; 6]);

// -------------- //
// Implémentation //
// -------------- //

impl MacAddr {
    fn new() -> MacAddr {
        let mut octets: [u8; 6] = [0; 6];
        rand::thread_rng().fill_bytes(&mut octets);
        octets[0] |= 0b_0000_0011;
        Self(octets)
    }

    fn is_local(&self) -> bool {
        (self.0[0] & 0b_0000_0010) == 0b_0000_0010
    }

    fn is_unicast(&self) -> bool {
        (self.0[0] & 0b_0000_0001) == 0b_0000_0001
    }
}

// -------------- //
// Implémentation // -> Interface
// -------------- //

impl Display for MacAddr {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let octet = &self.0;
        write!(
            f,
            "{:02x}:{:02x}:{:02x}:{:02x}:{:02x}:{:02x}",
            octet[0], octet[1], octet[2], octet[3], octet[4], octet[5]
        )
    }
}

// ----------- //
// Application //
// ----------- //

pub fn mac_addr() {
    let mac = MacAddr::new();

    assert!(mac.is_local());
    assert!(mac.is_unicast());

    println!("mac addresse: {}", mac);
}

mod macaddr;
mod memscan;

fn main() {
    memscan::memscan();
    macaddr::mac_addr();
}

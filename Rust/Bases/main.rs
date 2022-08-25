mod macaddr;
mod memscan;
mod smartptr;

fn main() {
    memscan::run();
    macaddr::run();
    smartptr::run();
}

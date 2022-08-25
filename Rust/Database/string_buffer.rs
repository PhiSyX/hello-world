use std::io;

use crate::REPL;

// Alias de String. Mais peut être amené à changer dans le futur.!?
pub(crate) type StringBuffer = String;

impl REPL {
    pub(crate) fn read_line(input: &mut StringBuffer) -> io::Result<()> {
        //                                            \n        \r\n
        let ends_size: usize = if cfg!(not(windows)) { 1 } else { 2 };
        io::stdin()
            .read_line(input)
            .map(|_| input.truncate(input.len() - ends_size))
    }
}

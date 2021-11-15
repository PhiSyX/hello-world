///! Simple CHIP-8 emulator

// --------- //
// Structure //
// --------- //

#[derive(Debug)]
struct Cpu {
    registers: [u8; 16],

    /// Position en mémoire.
    ///
    /// Utiliser usize plutôt que u16 diverge de la spécification.
    /// Mais on utilise usize car Rust permet de les utiliser pour
    /// l'indexation.
    position_memory: usize,

    /// Le CHIP-8 n'a que 4096 octets de RAM.
    /// Ca permet à l'équivalent du type usize de n'avoir qu'une largeur
    /// de 12 bits.
    memory: [u8; 4096],

    /// La hauteur (height) maximale de la pile (stack) est de 16. Après
    /// 16 appels de fonctions imbriquées, le programme rencontre un
    /// dépassement de pile (stack overflow).
    stack: [u16; 16],

    /// Le type usize est utilisé pour l'indexation de la pile.
    stack_ptr: usize,
}

// -------------- //
// Implémentation //
// -------------- //

impl Cpu {
    /// Pour créer un opcode de type [u16], nous devons combiner deux
    /// valeurs de la mémoire avec l'opérande logique OU (|). Ces valeurs
    /// doivent être exprimées en [u16] pour commencer; sinon le décalage
    /// vers la gauche met tous les bits à 0.
    fn read_opcode(&self) -> u16 {
        let position = self.position_memory;

        let op_byte1 = self.memory[position] as u16;
        let op_byte2 = self.memory[position + 1] as u16;

        (op_byte1 << 8) | op_byte2
    }

    fn run(&mut self) {
        loop {
            let opcode = self.read_opcode();

            let x = ((opcode & 0x0F00) >> 8) as u8;
            let y = ((opcode & 0x00F0) >> 4) as u8;

            let kk = (opcode & 0x00FF) as u8;
            let op_minor = (opcode & 0x000F) /* >> 0 */ as u8;
            let addr = opcode & 0x0FFF;

            self.position_memory += 2;

            match opcode {
                | 0x0000 => return,
                | 0x00E0 => {}

                | 0x00EE => self.ret(),
                | 0x1000..=0x1FFF => self.jmp(addr),
                | 0x2000..=0x2FFF => self.call(addr),
                | 0x3000..=0x3FFF => self.se(x, kk),
                | 0x4000..=0x4FFF => self.sne(x, kk),
                | 0x5000..=0x5FFF => self.se(x, y),
                | 0x6000..=0x6FFF => self.ld(x, kk),
                | 0x7000..=0x7FFF => self.add(x, kk),

                | 0x8000..=0x8FFF => match op_minor {
                    | 0 => self.ld(x, self.registers[y as usize]),
                    | 1 => self.or_xy(x, y),
                    | 2 => self.and_xy(x, y),
                    | 3 => self.xor_xy(x, y),
                    | 4 => self.add_xy(x, y),
                    | _ => todo!("opcode: {:04x}", opcode),
                },

                | _ => todo!("opcode {:04x}", opcode),
            }
        }
    }

    /// (7xkk) Add ajoute la valeur `kk` dans le registres `vx`
    fn add(&mut self, vx: u8, kk: u8) {
        self.registers[vx as usize] += kk;
    }

    // (7xkk)
    fn add_xy(&mut self, x: u8, y: u8) {
        self.registers[x as usize] += self.registers[y as usize];
    }

    fn and_xy(&mut self, x: u8, y: u8) {
        let x_ = self.registers[x as usize];
        let y_ = self.registers[y as usize];

        self.registers[x as usize] = x_ & y_;
    }

    /// (2nnn) CALL sub-routine -> `addr`
    fn call(&mut self, addr: u16) {
        let stack_ptr = self.stack_ptr;
        let stack = &mut self.stack;

        if stack_ptr > stack.len() {
            panic!("Stack overflow");
        }

        stack[stack_ptr] = self.position_memory as u16;
        self.stack_ptr += 1;
        self.position_memory = addr as usize;
    }

    /// (1nnn) JUMP -> `addr`
    fn jmp(&mut self, addr: u16) {
        self.position_memory = addr as usize;
    }

    /// (6xkk) LD défini la valeur `kk` dans le registre `vx`
    fn ld(&mut self, vx: u8, kk: u8) {
        self.registers[vx as usize] = kk;
    }

    /// (00ee) RET retourne à partir de la sub-routine courante
    fn ret(&mut self) {
        if self.stack_ptr == 0 {
            panic!("Stack underflow");
        }

        self.stack_ptr -= 1;
        let call_addr = self.stack[self.stack_ptr];
        self.position_memory = call_addr as usize;
    }

    fn or_xy(&mut self, x: u8, y: u8) {
        let x_ = self.registers[x as usize];
        let y_ = self.registers[y as usize];

        self.registers[x as usize] = x_ | y_;
    }

    fn se(&mut self, vx: u8, kk: u8) {
        if vx == kk {
            self.position_memory += 2;
        }
    }

    /// () SNE -> **S**tore if **n**ot **e**qual
    fn sne(&mut self, vx: u8, kk: u8) {
        if vx != kk {
            self.position_memory += 2;
        }
    }

    fn xor_xy(&mut self, x: u8, y: u8) {
        let x_ = self.registers[x as usize];
        let y_ = self.registers[y as usize];

        self.registers[x as usize] = x_ ^ y_;
    }
}

// ---- //
// Main //
// ---- //

fn main() {
    let mut cpu = Cpu {
        registers: [0; 16],
        memory: [0; 4096],
        position_memory: 0,
        stack: [0; 16],
        stack_ptr: 0,
    };

    cpu.registers[0] = 5;
    cpu.registers[1] = 10;

    let mem = &mut cpu.memory;
    mem[0x000] = 0x21; mem[0x001] = 0x00;
    mem[0x002] = 0x21; mem[0x003] = 0x00;

    mem[0x100] = 0x80; mem[0x101] = 0x14;
    mem[0x102] = 0x80; mem[0x103] = 0x14;
    mem[0x104] = 0x00; mem[0x105] = 0xEE;

    cpu.run();

    assert_eq!(cpu.registers[0], 45);

    println!("{:?}", cpu);
}

///! Simple CHIP-8 emulator

// --------- //
// Structure //
// --------- //

#[derive(Debug)]
struct Cpu {
    /// Tous les opcodes de CHIP-8 sont des `u16`.
    current_op: u16,
    registers: [u8; 2],
}

// -------------- //
// Implémentation //
// -------------- //

impl Cpu {
    fn run(&mut self) {
        let opcode = self.current_op;

        // Sélectionne des quarts de bit à l'aide de l'opérande AND (&)
        // pour filtrer les bits qui doivent être conservés, puis les
        // décale pour déplacer les bits vers la position la moins
        // significative. La notation hexadécimale est pratique
        // pour ces opérations car chaque chiffre représente 4
        // bits. Une valeur 0xF sélectionne tous les bits à partir
        // d'un agrégat de 4 bits (soit un demi octet).
        let c = ((opcode & 0xF000) >> 12) as u8;
        let x = ((opcode & 0x0F00) >> 8) as u8;
        let y = ((opcode & 0x00F0) >> 4) as u8;
        let d = (opcode & 0x000F) /* >> 0 */ as u8;

        match (c, x, y, d) {
            // Distribue l'exécution au circuit matériel chargé de
            // l'effectuer.
            | (0x8, _, _, 0x4) => self.add_xy(x, y),
            // Un émulateur complet contient plusieurs dizaines
            // d'opérations
            | _ => todo!("opcode {:04x}", opcode),
        }
    }

    fn add_xy(&mut self, x: u8, y: u8) {
        self.registers[x as usize] += self.registers[y as usize];
    }
}

// ---- //
// Main //
// ---- //

fn main() {
    let mut cpu = Cpu {
        current_op: 0, // no-op
        registers: [0; 2],
    };

    // Contexte:
    // La constante Ox8014 est l'opcode que le CPU va interpréter. Pour le
    // décoder, il faut le diviser en quatre parties:
    //    - 8 signifie que l'opération concerne deux registres
    //    - 0 correspond à l'index 0 du register `cpu.registers[0]`
    //    - 1 correspond à l'index 1 du register `cpu.registers[1]`
    //    - 4 indique l'ajout
    cpu.current_op = 0x8014;

    cpu.registers[0] = 5;
    cpu.registers[1] = 10;

    cpu.run();

    assert_eq!(cpu.registers[0], 15);
    assert_eq!(cpu.registers[1], 10);

    println!("{:?}", cpu);
}

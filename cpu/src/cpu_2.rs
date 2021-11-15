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

            // Incrémente la position de mémoire pour le prochain opcode.
            self.position_memory += 2;

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
                // Court-circuite la fonction pour terminer l'exécution
                // lorsque l'opcode 0x000 est rencontré.
                | (0, 0, 0, 0) => return,

                // Distribue l'exécution au circuit matériel chargé de
                // l'effectuer.
                | (0x8, _, _, 0x4) => self.add_xy(x, y),

                // Un émulateur complet contient plusieurs dizaines
                // d'opérations
                | _ => todo!("opcode {:04x}", opcode),
            }
        }
    }

    /// Gestion des overflow dans CHIP-8
    fn add_xy(&mut self, x: u8, y: u8) {
        let arg_1 = self.registers[x as usize];
        let arg_2 = self.registers[y as usize];

        // La méthode `overflowing_add` pour un type u8, retourne un tuple
        // de deux valeurs. La première valeur est la valeur
        // résultante, la deuxième valeur est un booléen qui indique
        // si l'opération a provoqué un overflow.
        let (val, overflow) = arg_1.overflowing_add(arg_2);

        self.registers[x as usize] = val;

        if overflow {
            self.registers[0xF] = 1;
        } else {
            self.registers[0xF] = 0;
        }
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
    };

    cpu.registers[0] = 5;
    cpu.registers[1] = 10;
    cpu.registers[2] = 10;
    cpu.registers[3] = 10;

    let mem = &mut cpu.memory;
    // 0x8014 qui ajoute le registre 1 au registre 0.
    mem[0] = 0x80; mem[1] = 0x14;
    // 0x8024 qui ajoute le registre 2 au registre 0.
    mem[2] = 0x80; mem[3] = 0x24;
    // 0x8034 qui ajoute le registre 3 au registre 0.
    mem[4] = 0x80; mem[5] = 0x34;

    cpu.run();

    assert_eq!(cpu.registers[0], 35);

    println!("{:?}", cpu);
}

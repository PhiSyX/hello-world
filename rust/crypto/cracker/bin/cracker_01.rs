use std::{
    env,
    fs::File,
    io::{BufRead, BufReader},
};

use sha1::Digest;

fn main() -> std::io::Result<()> {
    let args: Vec<String> = env::args().collect();

    if args.len() != 3 {
        println!("Utilisation:");
        println!("cargo r --bin sha1-cracker-01 -- <password_list.txt> <hash-SHA1-de-40-caractères>");
        return Ok(());
    }

    let hash_to_crack = args[2].trim();
    if hash_to_crack.len() != 40 {
        println!("La longueur du hash SHA1 n'est pas correcte");
        return Err(std::io::Error::from(std::io::ErrorKind::InvalidData));
    }

    let list = File::open(&args[1])?;
    let buf_reader = BufReader::new(&list);

    let password_found = buf_reader
        .lines()
        .filter_map(|maybe_line| maybe_line.ok())
        .find(|line| {
            let common_password = line.trim();
            let encoded_password =
                hex::encode(sha1::Sha1::digest(common_password));
            hash_to_crack == encoded_password
        });

    if let Some(password) = password_found {
        println!("Mot de passe trouvé: {password}");
    } else {
        println!("Aucun mot de passe trouvé dans la liste.")
    }

    Ok(())
}

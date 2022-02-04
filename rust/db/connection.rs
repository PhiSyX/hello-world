use std::fs::File;

pub(crate) struct Connection {
    fd: File,
}

impl Connection {
    pub(crate) fn open(database_filename: &str) -> Self {
        let fd = File::open(database_filename)
            .or_else(|_| File::create(database_filename))
            .expect("Erreur: impossible d'ouvrir le fichier.");

        Self { fd }
    }
}

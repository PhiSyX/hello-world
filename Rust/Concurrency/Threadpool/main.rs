use std::sync::mpsc;

use threadpool::ThreadPool;

fn main() {
    let total_workers = 4;
    let total_jobs = 8;

    let pool = ThreadPool::new(total_workers);

    let (tx, rx) = mpsc::channel();

    (0..total_jobs).for_each(|_| {
        let tx = tx.clone();

        pool.execute(move || {
            tx.send(1).expect("un envoie de donnée au thread pool");
        });
    });

    println!("Résultat: {}", rx.iter().take(total_jobs).sum::<usize>())
}

#![allow(dead_code, unused_variables, clippy::manual_async_fn)]

use std::{
    future::Future,
    io::{BufRead, Write},
};

fn main() {
    let terminal = read_from_terminal();
    let network = read_from_network();

    select! {
        line <- terminal.await => {
            // do something with line
        }
        stream <- network.await => {
            // do something on stream
        }
        foo <- foo2().await {
            // do something with foo
        }
    }
}

async fn read_to_string(s: &str) {}
fn expensive_fn(_: ()) {}

// this asynchronous function is equivalent to `foo2` function
async fn foo() -> usize {
    println!("foo");
    42
}

fn foo2() -> impl Future<Output = usize> {
    async {
        println!("foo");
        42
    }
}

fn foo3() -> impl Future<Output = usize> {
    async {
        //let x = read_to_string("file").await;

        let fut = read_to_string("file");
        let x = loop {
            if let Some(result) = fut.try_check_completed() {
                break result;
            } else {
                fut.try_make_progress();
                yield;
            }
        };

        println!("foo1");
        read_to_string("file1").await;

        println!("foo2");
        read_to_string("file2").await;

        println!("foo3");
        let x = /* waiting on */ read_to_string("file3").await;
        expensive_fn(x);

        /* yield again and wait on */
        println!("foo4");
        read_to_string("file4").await;

        42
    }
}

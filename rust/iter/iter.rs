#![allow(dead_code)]

use std::{thread::sleep, time::Duration};

const CLEAR: &str = "\x1B[2J\x1B[H";

// #1
fn progress_1(v: Vec<i32>) {
    let mut i = 1;
    for n in v.iter() {
        println!("{CLEAR}{}", "*".repeat(i));
        i += 1;
        expensive_calculation(n);
    }
}

// #2
fn progress_2<T>(v: Vec<T>, f: fn(&T) -> ()) {
    let mut i = 1;
    for n in v.iter() {
        println!("{CLEAR}{}", "*".repeat(i));
        i += 1;
        f(n);
    }
}

// #3
fn progress_3<Iter>(iter: Iter, f: fn(Iter::Item) -> ())
where
    Iter: Iterator,
{
    let mut i = 1;
    for n in iter {
        println!("{CLEAR}{}", "*".repeat(i));
        i += 1;
        f(n);
    }
}

// #4
struct Progress<Iter, Bound> {
    iter: Iter,
    cur: usize,
    // #6
    bound: Bound,
}

impl<Iter> Progress<Iter, Unbounded>
where
    Iter: Iterator,
{
    fn new(iter: Iter) -> Self {
        Self {
            iter,
            cur: 0,
            bound: Unbounded,
        }
    }
}

// #5
trait ProgressIteratorExt: Sized {
    fn progress(self) -> Progress<Self, Unbounded>;
}

impl<Iter> ProgressIteratorExt for Iter
where
    Iter: Iterator,
{
    fn progress(self) -> Progress<Self, Unbounded> {
        Progress::new(self)
    }
}

impl<Iter, Bound> Iterator for Progress<Iter, Bound>
where
    Iter: Iterator,
    Bound: ProgressDisplay,
{
    type Item = Iter::Item;

    fn next(&mut self) -> Option<Self::Item> {
        println!("{CLEAR}");
        self.bound.display(&self);
        self.cur += 1;
        self.iter.next()
    }
}

// #6
impl<Iter> Progress<Iter, Unbounded>
where
    Iter: ExactSizeIterator,
{
    fn with_bound(mut self) -> Progress<Iter, Bounded> {
        let bound = Bounded {
            bound: self.iter.len(),
            delimiters: ('[', ']'),
        };
        Progress {
            iter: self.iter,
            cur: self.cur,
            bound,
        }
    }
}

// #7
impl<Iter> Progress<Iter, Bounded>
where
    Iter: Iterator,
{
    fn with_delimiters(mut self, delimiters: (char, char)) -> Self {
        self.bound.delimiters = delimiters;
        self
    }
}

// #8

struct Unbounded;
struct Bounded {
    bound: usize,
    delimiters: (char, char),
}

trait ProgressDisplay: Sized {
    fn display<Iter>(&self, progress: &Progress<Iter, Self>);
}

impl ProgressDisplay for Unbounded {
    fn display<Iter>(&self, progress: &Progress<Iter, Self>) {
        println!("{}", "*".repeat(progress.cur));
    }
}

impl ProgressDisplay for Bounded {
    fn display<Iter>(&self, progress: &Progress<Iter, Self>) {
        println!(
            "{}{}{}{}",
            self.delimiters.0,
            "*".repeat(progress.cur),
            " ".repeat(self.bound - progress.cur),
            self.delimiters.1
        );
    }
}

fn main() {
    let v: Vec<i32> = vec![1, 2, 3];
    // On ne va pas garder les solutions commentées ci-dessous
    //progress_1(v);
    //progress_2(v, expensive_calculation);
    //progress_3(v.iter(), expensive_calculation);

    // #4
    //for n in Progress::new(v.iter()) {
    //    expensive_calculation(n);
    //}

    // #5
    // for n in v.iter().progress() {
    // #6
    // for n in v.iter().progress().with_bound() {
    // for n in (0..).progress().with_bound() { // -> erreur: normal.
    //     expensive_calculation(n);
    // }
    // #7
    //let brackets = ('{', '}');
    //for n in (0..).progress().with_delimiters(brackets) { // ok: n'est pas normal.
    //expensive_calculation(&n);
    //}
    // #8
    let brackets = ('{', '}');
    // for n in (0..).progress().with_delimiters(brackets) { // erreur: normal.
    for n in v.iter().progress().with_bound().with_delimiters(brackets) {
        expensive_calculation(&n);
    }
}

fn expensive_calculation(_n: &i32) {
    sleep(Duration::from_secs(1));
}

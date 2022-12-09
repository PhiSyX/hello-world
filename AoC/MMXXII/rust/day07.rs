use core::str;
use core::{
    cell::RefCell,
    fmt, mem,
    ops::{self, Deref},
};
use std::rc::Rc;

use itertools::*;
use mmxxii_rust::solved_output;

const PUZZLE: &str = include_str!("../puzzles/day07.txt");

#[derive(Debug)]
enum Command {
    LS { output: Files },
    CD { path: String },
}

impl str::FromStr for Command {
    type Err = ();

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        if s.starts_with("cd ") {
            let mut parts = s.split_ascii_whitespace();
            parts.next();

            let path = parts.next().expect("chemin de la commande");

            return Ok(Self::CD {
                path: path.to_owned(),
            });
        } else if s.starts_with("ls\n") {
            let Some((_, parts)) = s.split_once('\n') else {
                return Err(())
            };

            let output = parts.parse::<Files>().expect("fichiers");

            return Ok(Self::LS { output });
        }

        dbg!(&s);

        Err(())
    }
}

#[derive(Debug)]
struct Files(Vec<File>);

impl str::FromStr for Files {
    type Err = ();

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        Ok(Self(
            s.lines()
                .filter_map(|line| line.parse::<File>().ok())
                .collect(),
        ))
    }
}

impl ops::Deref for Files {
    type Target = Vec<File>;

    fn deref(&self) -> &Self::Target {
        &self.0
    }
}

#[derive(Debug, PartialEq, Clone)]
enum File {
    File { size: usize, name: String },
    Dir { name: String },
}

impl File {
    fn name(&self) -> &str {
        match self {
            | Self::File { name, .. } | Self::Dir { name } => name,
        }
    }
}

impl str::FromStr for File {
    type Err = ();

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let mut parts = s.split_ascii_whitespace();

        if s.starts_with("dir ") {
            parts.next();

            let dirname = parts.next().expect("dirname");

            return Ok(Self::Dir {
                name: dirname.to_owned(),
            });
        }

        let filesize = parts
            .next()
            .and_then(|s| s.parse().ok())
            .expect("file size");
        let filename = parts.next().expect("file name");

        Ok(Self::File {
            size: filesize,
            name: filename.to_owned(),
        })
    }
}

#[derive(Debug)]
struct Node {
    value: File,
    parent: Option<Rc<Self>>,
    children: RefCell<Vec<Rc<Self>>>,
}

impl Node {
    fn new() -> Rc<Self> {
        Rc::new(Self {
            value: File::Dir { name: "/".into() },
            parent: Default::default(),
            children: Default::default(),
        })
    }

    fn change_directory(self: &mut Rc<Self>, pathname: &str) {
        if pathname == ".." {
            let parent = self.parent.as_ref().cloned().expect("Parent");
            _ = mem::replace(self, parent);
        } else if pathname == "/" {
            while self.parent.is_some() {
                self.change_directory("..");
            }
        } else {
            let child = self
                .children
                .borrow()
                .iter()
                .find(|dir| dir.name() == pathname)
                .expect("Noeud")
                .clone();
            _ = mem::replace(self, child);
        }
    }

    fn create_file(self: &Rc<Self>, file: &File) {
        self.children.borrow_mut().push(Rc::new(Self {
            value: file.to_owned(),
            parent: Some(self.clone()),
            children: Default::default(),
        }));
    }

    fn size(&self) -> usize {
        match self.value {
            | File::File { .. } => return 0,
            | File::Dir { .. } => (),
        }

        self.children
            .borrow()
            .iter()
            .map(|child| match child.value {
                | File::File { size, .. } => size,
                | File::Dir { .. } => child.size(),
            })
            .sum::<usize>()
    }

    fn traverse<'a>(self: &'a Rc<Self>) -> Vec<Rc<Self>> {
        let mut vector = self
            .children
            .borrow()
            .iter()
            .map(|node| node.traverse())
            .fold(Vec::new(), |mut vector, mut cur| {
                vector.append(&mut cur);
                vector
            });
        vector.push(self.clone());
        vector
    }

    fn name<'a>(self: &'a Rc<Self>) -> &'a str {
        self.value.name()
    }
}

fn main() {
    println!("--- Day 7: No Space Left On Device ---");

    let part01 = solve_part01(PUZZLE);
    println!("--- Part One ---");
    println!("\tYour puzzle answer is {}.", solved_output(part01));

    let part02 = solve_part02(PUZZLE);
    println!("--- Part Two ---");
    println!("\tYour puzzle answer is {}.", solved_output(part02));
}

fn build_tree(input: &'static str) -> Rc<Node> {
    let cmds: Vec<_> = input
        .split("$ ")
        .map(|cmd| cmd.trim())
        .filter(|cmd| !cmd.is_empty())
        .filter_map(|cmd| cmd.parse::<Command>().ok())
        .collect();

    let mut fd = Node::new();

    for cmd in cmds.into_iter().skip(1) {
        match cmd {
            | Command::CD { path } => fd.change_directory(&path),
            | Command::LS { output } => {
                for file in output.deref() {
                    fd.create_file(file);
                }
            }
        }
    }

    fd.change_directory("/");

    fd
}

fn solve_part01(input: &'static str) -> impl fmt::Display {
    build_tree(input)
        .traverse()
        .iter()
        .fold(0, |mut total, node| {
            let size = node.size();

            if size < 100_000 {
                total += node.size();
            }

            total
        })
}

fn solve_part02(input: &'static str) -> impl fmt::Display {
    let tree = build_tree(input);
    let total_size = tree.size();

    let folder_size = 70_000_000;
    let remaining_space = folder_size - total_size;
    let needed_space = 30_000_000 - remaining_space;

    tree.traverse()
        .into_iter()
        .map(|dir| dir.size())
        .filter(|size| *size > needed_space)
        .sorted()
        .next()
        .unwrap_or_default()
}

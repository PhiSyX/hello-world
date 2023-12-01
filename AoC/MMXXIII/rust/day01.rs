use mmxxiii_rust::{filter_map_line, solved_output};

const PUZZLE: &str = include_str!("../puzzles/day01.txt");

const NUMERAL_WORDS: [&str; 10] = [
    "zero", "one", "two", "three", "four", "five", "six", "seven",
    "eight", "nine",
];

fn solve_part01(input: &'static str) -> u32 {
    filter_map_line(input, retrieves_first_and_last_digits).sum::<u32>()
}

fn retrieves_first_and_last_digits(line: &str) -> Option<u32> {
    let mut it = line
        .chars()
        .filter(char::is_ascii_digit)
        .map(|ch| (ch as u8 - b'0') as u32);
    let f = it.next()?;
    let l = it.last().unwrap_or(f);
    Some(f * 10 + l)
}

fn solve_part02(input: &'static str) -> u32 {
    filter_map_line(input, parse_numeral_words).sum::<u32>()
}

fn parse_numeral_words(line: &str) -> Option<u32> {
    let mut line = String::from(line);

    for (n, word) in NUMERAL_WORDS.iter().enumerate() {
        let s = format!("{}{}{}", &word[..1], n, &word[word.len() - 1..]);
        line = line.replace(word, &s);
    }

    retrieves_first_and_last_digits(&line)
}

fn main() {
    println!("--- Day 1: Trebuchet?! ---");

    let part01 = solve_part01(PUZZLE);
    println!("--- Part One ---");
    println!("\tYour puzzle answer is {}.", solved_output(part01));

    let part02 = solve_part02(PUZZLE);
    println!("--- Part Two ---");
    println!("\tYour puzzle answer is {}.", solved_output(part02));
}

// ---- //
// Test //
// ---- //

#[cfg(test)]
mod tests {
    use super::*;

    #[test]

    fn test_example_01_01() {
        const PUZZLE: &str = "1abc2
pqr3stu8vwx
a1b2c3d4e5f
treb7uchet";

        let part01 = solve_part01(PUZZLE);
        assert_eq!(part01, 142);
    }

    #[test]
    fn test_example_01_02() {
        const PUZZLE: &str = "two1nine
eightwothree
abcone2threexyz
xtwone3four
4nineeightseven2
zoneight234
7pqrstsixteen";
        let part02 = solve_part02(PUZZLE);
        assert_eq!(part02, 281);
    }
}

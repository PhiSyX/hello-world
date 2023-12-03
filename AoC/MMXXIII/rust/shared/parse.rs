pub fn filter_line(input: &str, filter_fn: fn(&str) -> bool) -> impl Iterator<Item = &str>
{
	input.lines().filter(move |line| filter_fn(line))
}

pub fn filter_map_line<T>(input: &'static str, filter_fn: fn(&str) -> Option<T>) -> impl Iterator<Item = T>
{
	input.lines().filter_map(filter_fn)
}

pub fn charsln_iter(input: &'static str) -> impl Iterator<Item = Vec<char>> + 'static
{
	input.lines().map(|line| line.chars().collect())
}

pub fn charsln(input: &'static str) -> Vec<Vec<char>>
{
	charsln_iter(input).collect()
}

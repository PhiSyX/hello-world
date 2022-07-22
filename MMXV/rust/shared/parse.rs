pub fn filter_line(
	input: &str,
	filter_fn: fn(&str) -> bool,
) -> impl Iterator<Item = &str> {
	input.lines().filter(move |line| filter_fn(line))
}

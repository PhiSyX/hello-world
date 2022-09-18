pub fn filter_line(
	input: &str,
	filter_fn: fn(&str) -> bool,
) -> impl Iterator<Item = &str> {
	input.lines().filter(move |line| filter_fn(line))
}

pub fn filter_map_line<T>(
	input: &'static str,
	filter_fn: fn(&str) -> Option<T>,
) -> impl Iterator<Item = T> {
	input.lines().filter_map(filter_fn)
}

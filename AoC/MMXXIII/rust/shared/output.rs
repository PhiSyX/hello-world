const PREFIX_FORMAT: &str = "\x1b[";
const SUFFIX_FORMAT: &str = "m";

fn format_text(text: impl ToString, open: u8, close: u8) -> String {
	format!(
		"{p}{open}{s}{text}{p}{close}{s}",
		open = open,
		text = text.to_string(),
		close = close,
		p = PREFIX_FORMAT,
		s = SUFFIX_FORMAT,
	)
}

fn bold(text: impl ToString) -> String {
	format_text(text, 1, 22)
}

fn blink(text: impl ToString) -> String {
	format_text(text, 5, 25)
}

pub fn solved_output(text: impl ToString) -> String {
	blink(bold(format_text(text, 33, 0)))
}

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

typedef unsigned int usize;
typedef int isize;

constexpr isize
from_ch(const char ch)
{
	if ((ch < 'a') || (ch > 'z')) {
		return -1;
	}
	return (ch - 'a');
}

constexpr char
to_ch(const isize u)
{
	if (u > 26 || u < 0) {
		return '-';
	}
	return 'a' + u;
}

const bool
check_requirements(std::vector<isize> const& indexes)
{
	for (usize i = 2; i < indexes.size(); ++i) {
		if ((indexes[i - 2] + 2) == indexes[i] &&
			(indexes[i - 1] + 1) == indexes[i]) {
			return true;
		}
	}
	return false;
}

void
incr_passwd(std::vector<isize>& indexes)
{
	const isize index = indexes.size() - 1;
	++indexes[index];
	for (usize i = 0; i < indexes.size(); ++i) {
		isize current_index = index - i;
		if (current_index < 0) {
			current_index += indexes.size();
		}
		if (indexes[current_index] == 26) {
			indexes[current_index] %= 26;

			if (current_index == 0) {
				++indexes[indexes.size() - 1];
			} else {
				++indexes[current_index - 1];
			}
		} else {
			break;
		}
	}
}

void
incr_indexes(std::vector<isize>& indexes)
{
	for (usize i = 0; i < indexes.size(); ++i) {
		if (from_ch('o') == indexes[i] || from_ch('i') == indexes[i] ||
			from_ch('l') == indexes[i]) {

			++indexes[i];

			for (usize j = i + 1; j < indexes.size(); ++j) {
				indexes[j] = 0;
			}
		}
	}
}

const bool
contains_banned_chars(std::vector<isize> const& indexes)
{
	for (auto const& u : indexes) {
		if (from_ch('o') == u || from_ch('i') == u || from_ch('l') == u) {
			return true;
		}
	}
	return false;
}

const bool
check_pairs(std::vector<int> const& indexes)
{
	isize pair = -1;
	for (usize i = 1; i < indexes.size(); ++i) {
		if (indexes[i - 1] == indexes[i]) {
			if (pair == -1) {
				pair = indexes[i];
				++i;
			} else if (indexes[i] != pair) {
				return true;
			} else {
				pair = indexes[i];
			}
		}
	}
	return false;
}

const std::string
get_passwd(std::string const& input)
{
	std::vector<isize> indexes;

	for (auto const& ch : input) {
		indexes.push_back(from_ch(ch));
	}

	bool is_valid{ false };

	while (!is_valid) {
		if (!check_requirements(indexes)) {
			incr_passwd(indexes);
			continue;
		}

		if (contains_banned_chars(indexes)) {
			incr_indexes(indexes);
			continue;
		}

		if (!check_pairs(indexes)) {
			incr_passwd(indexes);
			continue;
		}

		is_valid = true;
	}

	std::string output;

	for (auto const& u : indexes) {
		output += to_ch(u);
	}

	return output;
}

const std::string
solve_part01(std::string const& input)
{
	return get_passwd(input);
}

int
main(int argc, char* argv[])
{
	std::cout << "--- Day 11: Corporate Policy ---" << std::endl;

	std::ifstream puzzle{ "../puzzles/day11.txt" };
	std::string input{ std::istreambuf_iterator<char>(puzzle),
					   std::istreambuf_iterator<char>() };

	input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());

	if (input.size() == 0) {
		std::cout << "Le mot de passe n'est pas définie." << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "--- Part One ---" << std::endl;
	auto const part01{ solve_part01(input) };
	std::cout << "\t Your puzzle answer is " << part01 << "." << std::endl;

	return EXIT_SUCCESS;
}
#include <iostream>
#include <span>
#include <vector>

void
print_span(std::span<int> span)
{
	for (auto item : span) {
		std::cout << item << ' ';
	}

	std::cout << '\n';
}

int
main()
{
	std::vector<int> my_vector = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	print_span(std::span(my_vector.begin() + 1, 5));
	return 0;
}
#include <algorithm>
#include <iostream>
#include <vector>

int
main()
{
	auto is_divisable_by = [divisor = 10](int dividend) {
		return dividend % divisor == 0;
	};

	std::vector<int> my_vector = { 88, 40, 50, 24, 37, 15 };
	auto itr = std::ranges::find_if(my_vector, is_divisable_by);
	std::cout << *itr << '\n';
	return 0;
}
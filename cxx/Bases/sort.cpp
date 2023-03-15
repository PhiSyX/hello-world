#include <algorithm>
#include <array>
#include <iostream>

void
print(auto arr)
{
	for (auto item : arr) {
		std::cout << item << ' ';
	}
	std::cout << '\n';
}

int
main()
{
	std::array<int, 5> arr = { 12, 42, 7, 99, 1 };
	print(arr);
	std::ranges::sort(arr);
	print(arr);
	return 0;
}

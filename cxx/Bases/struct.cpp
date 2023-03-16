#include <algorithm>
#include <iostream>
#include <vector>

struct Math
{
	int divisor;

	Math(int n)
	  : divisor(n)
	{
	}

	bool operator()(int dividend) { return dividend % divisor == 0; }
};

int
main()
{
	Math is_divisable_by(10);
	std::vector<int> arr = { 88, 40, 50, 24, 37, 15 };
	auto itr = std::ranges::find_if(arr, is_divisable_by);
	std::cout << *itr << '\n';
	return 0;
}
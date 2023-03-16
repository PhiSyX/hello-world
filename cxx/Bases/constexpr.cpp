#include <iostream>
#include <random>

constexpr int
factorial(int n)
{
	if (n <= 1) {
		return 1;
	} else {
		return n * factorial(n - 1);
	}
}

int
main()
{
	std::random_device rd;
	int output = factorial(rd() % 12);
	std::cout << output << '\n';
	return 0;
}

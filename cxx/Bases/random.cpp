#include <iostream>
#include <random>

int
main()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution unid(1, 20);

	for (int i = 0; i < 20; i += 1) {
		std::cout << unid(mt) << ' ';
	}

	std::cout << '\n';

	return 0;
}

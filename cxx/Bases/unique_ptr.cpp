#include <iostream>
#include <memory>

int
main()
{
	auto ptr = std::make_unique<int[]>(10);

	for (int i = 0; i < 10; i += 1) {
		ptr[i] = i * i;
	}

	std::cout << ptr[3] << '\n';
	std::cout << ptr[7] << '\n';

	return 0;
}

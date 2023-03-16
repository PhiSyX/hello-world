#include <iostream>
#include <memory>
#include <utility>

int
main()
{
	auto ptr1 = std::make_unique<int[]>(42);
	std::cout << "ptr1 avant le déplacement: " << ptr1.get() << '\n';
	auto ptr2 = std::move(ptr1);
	std::cout << "ptr1 après le déplacement: " << ptr1.get() << '\n';
	std::cout << "ptr2 après le déplacement: " << ptr2.get() << '\n';

	return 0;
}
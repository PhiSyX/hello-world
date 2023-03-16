#include <fstream>
#include <iostream>

int
main()
{
	std::ofstream os("stream.txt");
	for (int i = 0; i < 1'000; i++) {
		os << i * i << ' ';
	}
	os << '\n' << std::endl;

	std::ifstream is("stream.txt");
	int data;
	while (is >> data) {
		std::cout << data << ' ';
	}
	std::cout << '\n';

	return 0;
}
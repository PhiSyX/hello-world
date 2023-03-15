#include <iostream>
#include <vector>

void
push_item(std::vector<int>& arr, int N)
{
	for (int i = 0; i < N; i += 1) {
		arr.push_back(i);
	}
}

int
main()
{
	std::vector<int> arr;

	push_item(arr, 10);

	for (auto item : arr) {
		std::cout << item << ' ';
	}

	std::cout << '\n';

	return 0;
}
#include <iostream>
#include <vector>

void
print(std::vector<int> arr)
{
	for (auto item : arr) {
		std::cout << item << ' ';
	}

	std::cout << '\n';
}

struct MyStruct
{
	int num;

	MyStruct(int n)
	  : num(n)
	{
		std::cout << "-------------\n";
		std::cout << "Ctor\n";
	}

	MyStruct(const MyStruct& ms)
	  : num(ms.num)
	{
		std::cout << "Copy " << ms.num << "\n";
	}

	MyStruct(MyStruct&& ms)
	  : num(ms.num)
	{
		std::cout << "Move " << ms.num << "\n";
	}
};

int
main()
{
	std::vector<int> my_vector1;

	my_vector1.reserve(10);

	for (int i = 0; i < 10; i += 1) {
		std::cout << "Size: " << my_vector1.size() << '\n';
		std::cout << "Capacity: " << my_vector1.capacity() << '\n';
		my_vector1.push_back(i);
	}

	std::vector<MyStruct> my_vector2;
	MyStruct ms1(40);
	my_vector2.push_back(ms1);

	MyStruct ms2(41);
	my_vector2.push_back(std::move(ms2));

	my_vector2.push_back(MyStruct(42));

	MyStruct ms3(43);
	my_vector2.push_back(ms3);

	my_vector2.emplace_back(44);

	return 0;
}
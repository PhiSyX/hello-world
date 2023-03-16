#include <atomic>
#include <iostream>
#include <thread>

int
main()
{
	std::atomic<int> counter = 0;

	auto handle = [&counter]() {
		for (int i = 0; i < 10'000'000; i += 1) {
			counter += 1;
		}
	};

	std::thread thread1(handle);
	std::thread thread2(handle);

	thread1.join();
	thread2.join();

	std::cout << counter << '\n';

	return 0;
}

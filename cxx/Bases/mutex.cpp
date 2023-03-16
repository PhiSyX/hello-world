#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

int
main()
{
	std::mutex mutex;

	auto thread_id = [&mutex](int id) {
		std::lock_guard<std::mutex> guard(mutex);

		std::cout << "Thread ID: " << id << '\n';
	};

	std::vector<std::jthread> threads;

	for (int i = 0; i < 10; i += 1) {
		threads.emplace_back(thread_id, i);
	}

	return 0;
}

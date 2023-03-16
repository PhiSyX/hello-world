#include <iostream>
#include <thread>
#include <vector>

void
thread_id(int id)
{
	std::cout << "Thread ID: " << id << '\n';
}

int
main()
{
	std::vector<std::jthread> threads;
	for (int i = 0; i < 10; i += 1) {
		threads.emplace_back(thread_id, i);
	}

	return 0;
}
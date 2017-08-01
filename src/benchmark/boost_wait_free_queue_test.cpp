#include "utils/perf_test_helper.h"
#include <boost/lockfree/spsc_queue.hpp>
#include <thread>

constexpr int CAPACITY = 1024 * 1024;

void consume(long long iteration, boost::lockfree::spsc_queue<int, boost::lockfree::capacity<CAPACITY>> *q) {
  long long count = 0;
  do {
	  int e;
    q->pop(e);
    count ++;
  } while (count < iteration);
}

void run(int pass, long long iteration) {
	PerfTestHelper helper;
	boost::lockfree::spsc_queue<int, boost::lockfree::capacity<CAPACITY>> q;
	std::thread t(consume, iteration, &q);
	helper.Register();
	for (long long i = 0; i < iteration; i ++) {
		q.push(1);
	}
	t.join();
	helper.Finish(pass, iteration);
}

int main() {
	std::cout << "Boost Wait Free Queue Test" << std::endl;
	int pass = 5;
	long long iteration = 1000 * 1000 * 10;
	for (int i = 1; i <= pass; i ++) {
		run(i, iteration);
	}
	return 0;
}

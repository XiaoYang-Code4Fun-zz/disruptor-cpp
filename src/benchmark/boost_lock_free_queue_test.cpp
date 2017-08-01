#include "utils/perf_test_helper.h"
#include <boost/lockfree/queue.hpp>
#include <thread>

void consume(long long iteration, boost::lockfree::queue<int> *q) {
  long long count = 0;
  do {
	  int e;
    q->pop(e);
    count ++;
  } while (count < iteration);
}

void run(int pass, long long iteration) {
	PerfTestHelper helper;
	boost::lockfree::queue<int> q(1024);
	std::thread t(consume, iteration, &q);
	helper.Register();
	for (long long i = 0; i < iteration; i ++) {
		q.push(1);
	}
	t.join();
	helper.Finish(pass, iteration);
}

int main() {
	std::cout << "Boost Lock Free Queue Test" << std::endl;
	int pass = 5;
	long long iteration = 1000 * 1000;
	for (int i = 1; i <= pass; i ++) {
		run(i, iteration);
	}
	return 0;
}

#include "test_setup.h"
#include "utils/perf_test_helper.h"
#include <thread>

namespace disruptor_cpp {

void run(int pass, int batch_size, int capacity, long long iteration) {
	int multipler = 3;
	PerfTestHelper helper;
	IntEventFactory factory;
	Disruptor<int> disruptor(capacity, MULTIPLE, YIELDING, &factory);
	IntEventHandler handler1(iteration * multipler);
	IntEventHandler handler2(iteration * multipler);
	IntEventHandler handler3(iteration * multipler);
	disruptor.HandleWith(&handler1);
	disruptor.HandleWith(&handler2);
	disruptor.HandleWith(&handler3);
	disruptor.Start();
	helper.Register();
	std::thread t[2];
	t[0] = std::thread(produce, batch_size, iteration, &factory, &disruptor);
	t[1] = std::thread(produce, batch_size, iteration, &factory, &disruptor);
	produce(batch_size, iteration, &factory, &disruptor);
	t[0].join();
	t[1].join();
	disruptor.WaitUntilFinish();
	helper.Finish(pass, iteration, multipler);
}

}

int main() {
	std::cout << "Many To Many Test" << std::endl;
	int pass = 5;
	for (int i = 1; i <= pass; i ++) {
		disruptor_cpp::run(i, 10, 4096, 1000 * 1000);
	}
	return 0;
}

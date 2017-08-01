#include "test_setup.h"
#include "utils/perf_test_helper.h"

namespace disruptor_cpp {
void run(int pass, int batch_size, int capacity, int64 iteration) {
	PerfTestHelper helper;
	IntEventFactory factory;
	Disruptor<int> disruptor(capacity, SINGLE, YIELDING, &factory);
	IntEventHandler handler(iteration);
	disruptor.HandleWith(&handler);
	disruptor.Start();
	helper.Register();
	produce(batch_size, iteration, &factory, &disruptor);
	disruptor.WaitUntilFinish();
	helper.Finish(pass, iteration);
}
}

int main() {
	std::cout << "One To One Test" << std::endl;
	int pass = 5;
	for (int i = 1; i <= pass; i ++) {
		disruptor_cpp::run(i, 100, 1024 * 1024, 1000 * 1000);
	}
	return 0;
}

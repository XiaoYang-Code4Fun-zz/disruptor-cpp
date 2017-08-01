#ifndef DISRUPTOR_CPP_INCLUDE_PERF_TEST_HELPER_H_
#define DISRUPTOR_CPP_INCLUDE_PERF_TEST_HELPER_H_

#include <chrono>
#include <iostream>
#include <locale>

class PerfTestHelper {
public:
	void Register() {
		start = std::chrono::system_clock::now();
	}
	void Finish(int pass, long long iteration, long long multipler = 1) {
		end = std::chrono::system_clock::now();
		auto elapse = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		std::cout.imbue(std::locale(""));
		std::cout << "Run pass " << pass << " with throughput " <<
			iteration * multipler * 1000 / elapse.count() << " ops/sec." << std::endl;
	}
private:
	std::chrono::time_point<std::chrono::system_clock> start, end;
};

#endif // DISRUPTOR_CPP_INCLUDE_PERF_TEST_HELPER_H_

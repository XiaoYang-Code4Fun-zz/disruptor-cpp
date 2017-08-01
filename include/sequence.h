#ifndef DISRUPTOR_CPP_INCLUDE_SEQUENCE_H_
#define DISRUPTOR_CPP_INCLUDE_SEQUENCE_H_

#include "common.h"
#include <atomic>
#include <vector>

namespace disruptor_cpp {
namespace {
constexpr int64 INITIAL_VALUE = -1L;
} // namespace

class Sequence {
public:
	static int64 InitialValue() {
			return INITIAL_VALUE;
		}
	static int64 GetMinimumSequence(const std::vector<Sequence*> &sequences, int64 default_value) {
		if (sequences.empty()) {
			return default_value;
		} else {
			int64 min = sequences[0]->Get();
			int size = sequences.size();
			for (int i = 1; i < size; i ++) {
				int64 value = sequences[i]->Get();
				if (min > value) {
					min = value;
				}
			}
			return min;
		}
	}
	Sequence() : value(INITIAL_VALUE) {}
	Sequence(int64 v): value(v) {}
	int64 Get() {
		return value;
	}
	void Set(int64 v) {
		value = v;
	}
	/*
	 * Atomically do compare_and_set and return whether it succeeds.
	 * If it fails, the expected value will be set to the new atomic value.
	 */
	bool CompareAndSet(int64 *expected, int64 desired) {
		return value.compare_exchange_strong(*expected, desired);
	}
	/**
	 * Atomically increment the sequence and return result.
	 */
	int64 IncrementAndGet() {
		return AddAndGet(1);
	}
	/**
	 * Atomically add the supplied value and return result.
	 */
	int64 AddAndGet(int64 v) {
		int64 current_value = Get();
		int64 new_value;
		do {
			new_value = current_value + v;
		} while (!(CompareAndSet(&current_value, new_value)));
		return new_value;
	}
private:
	std::atomic<int64> value;

};
} // namespace disruptor_cpp


#endif // DISRUPTOR_CPP_INCLUDE_SEQUENCE_H_

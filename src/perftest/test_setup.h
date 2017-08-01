#ifndef DISRUPTOR_CPP_TEST_SETUP_H_
#define DISRUPTOR_CPP_TEST_SETUP_H_

#include "disruptor_api.h"

namespace disruptor_cpp {

constexpr int64 PASS = 5;

class IntEventFactory : public EventFactory<int> {
public:
	int NewInstance() override {
		return 0;
	}
	void CopyEvent(int &dst, int src) override {
		dst = src;
	}
	void ClearEvent(int event) override {}
};

class IntEventHandler : public EventHandler<int> {
public:
	IntEventHandler(int64 iteration) : data_(0) , count_(0), iteration_(iteration) {}
	void OnStart() override {}
	void OnEvent(int event, int64 sequence, bool end_of_batch) throw(ExitException) override {
		data_ += event;
		count_ ++;
		if (count_ == iteration_) {
			throw ExitException();
		}
	}
	void OnShutDown() override {}
	int64 GetData() const { return data_; }
private:
	int64 data_;
	int64 count_;
	int64 iteration_;
};

void produce(int batch_size, int64 iteration, EventFactory<int> *factory, Disruptor<int> *disruptor) {
	int64 loop = 0;
	while (loop < iteration) {
		SequenceRange range = disruptor->Next(batch_size);
		for (int64 sequence = range.Low(); sequence <= range.High(); sequence ++) {
			int event = factory->NewInstance();
			event = 1;
			disruptor->Update(sequence, event);
		}
		disruptor->BatchPublish(range);
		loop += range.Size();
	}
}
}

#endif // DISRUPTOR_CPP_TEST_SETUP_H_

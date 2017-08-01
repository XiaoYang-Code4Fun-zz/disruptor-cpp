#ifndef DISRUPTOR_CPP_INCLUDE_RING_BUFFER_H_
#define DISRUPTOR_CPP_INCLUDE_RING_BUFFER_H_

#include "common.h"
#include "event_factory.h"
#include "exceptions/initialization_exception.h"
#include "sequence.h"
#include <iostream>
#include <math.h>

namespace disruptor_cpp {
template <typename T>
class RingBuffer {
public:
	RingBuffer() = delete;
	RingBuffer(int buffer_size, EventFactory<T> *event_factory) :
		buffer_size_(buffer_size), cursor_(Sequence::InitialValue()), event_factory_(event_factory) {
		if (buffer_size < 1) {
			throw InitializationException("Buffer size cannot be less than 1");
		}
		if ((buffer_size & (buffer_size - 1)) != 0) {
			throw InitializationException("Buffer size must be a power of 2");
		}
		event_buffer_ = new T[buffer_size];
		bit_width_ = (int) log2(buffer_size);
		FillEventBuffer();
	}
	~RingBuffer() {
		for (int i = 0; i < buffer_size_; i ++) {
			event_factory_->ClearEvent(event_buffer_[i]);
		}
		delete event_buffer_;
	}
	int GetBufferSize() {
		return buffer_size_;
	}
	Sequence* GetCursor() {
		return &cursor_;
	}
	T GetEvent(int64 sequence) {
		return event_buffer_[CalculateIndex(sequence)];
	}
	void StoreEvent(int64 sequence, T event) {
		event_factory_->CopyEvent(event_buffer_[CalculateIndex(sequence)], event);
	}
	int CalculateIndex(int64 sequence) {
		while ((sequence >> bit_width_) > 0) {
			sequence = sequence >> bit_width_;
		}
		return sequence;
	}
private:
	void FillEventBuffer() {
		for (int i = 0; i < buffer_size_; i ++) {
			event_buffer_[i] = event_factory_->NewInstance();
		}
	}
	int buffer_size_;
	Sequence cursor_;
	EventFactory<T> *event_factory_;
	T *event_buffer_;
	int bit_width_;
};
} // namespace disruptor_cpp

#endif // DISRUPTOR_CPP_INCLUDE_RING_BUFFER_H_

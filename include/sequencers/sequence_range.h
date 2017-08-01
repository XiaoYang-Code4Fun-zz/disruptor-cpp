#ifndef DISRUPTOR_CPP_INCLUDE_SEQUENCE_RANGE_H_
#define DISRUPTOR_CPP_INCLUDE_SEQUENCE_RANGE_H_

#include "common.h"

/*
 * A close sequence range in the ring buffer. The producer which obtains this range owns
 * all the slots in it, and is responsible to publish events in those slots - otherwise,
 * the cursor may not advance.
 */
namespace disruptor_cpp {
class SequenceRange {
public:
	SequenceRange() = delete;
	SequenceRange(int64 low, int64 high) : low_(low), high_(high) {}
	SequenceRange(const SequenceRange &range) : low_(range.low_), high_(range.high_) {}
	SequenceRange(SequenceRange &&range) : low_(range.low_), high_(range.high_) {}
	int64 Low() const { return low_; }
	int64 High() const { return high_; }
	int64 Size() const { return high_ - low_ + 1; }
private:
	int64 low_;
	int64 high_;
};
} // namespace disruptor_cpp

#endif // DISRUPTOR_CPP_INCLUDE_SEQUENCE_RANGE_H_

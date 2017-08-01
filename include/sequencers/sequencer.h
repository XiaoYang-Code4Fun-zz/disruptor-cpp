#ifndef DISRUPTOR_CPP_INCLUDE_SEQUENCER_H_
#define DISRUPTOR_CPP_INCLUDE_SEQUENCER_H_

#include "common.h"
#include "sequence.h"
#include "sequencers/sequence_range.h"
#include <memory>

namespace disruptor_cpp {
namespace {
constexpr int64 INTIAL_CURSOR_VALUE = -1L;
} // namespace

/*
 * Handles sequence claiming and dependency tracking.
 */
class Sequencer {
public:
	virtual ~Sequencer() {}
	/*
	 * Add list of gating sequence. This can only be called before running.
	 */
	virtual void AddGatingSequences(Sequence* sequence) = 0;
	/*
	 * Claims the next n slots in the ring buffer.
	 */
	virtual SequenceRange Next(int n) = 0;
	/*
	 * Publishes a sequence in the ring buffer.
	 */
	virtual void Publish(int64 sequence) = 0;
	/*
	 * Batch publishes a range in the ring buffer.
	 */
	virtual void BatchPublish(const SequenceRange &range) = 0;
};
} // namespace disruptor_cpp

#endif // DISRUPTOR_CPP_INCLUDE_SEQUENCER_H_

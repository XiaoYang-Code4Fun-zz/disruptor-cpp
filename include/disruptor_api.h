#ifndef DISRUPTOR_CPP_INCLUDE_DISRUPTOR_API_H_
#define DISRUPTOR_CPP_INCLUDE_DISRUPTOR_API_H_

/*
 * This header file includes all necessary components for using disruptor.
 *
 * First implement your own EvenFactory and EventHandler. Take FooEventFactory
 * and FooEventHandler for example.
 *
 * Then create your disruptor class:
 * Disruptor<Foo> disruptor(capacity, producer_type, wait_strategy, &foo_event_factory);
 * Argument producer_type can be SINGLE or MULTIPLE. Use SINGLE if you have only one
 * producer thread since that contains optimization specifically for the SINGLE case.
 * Argument wait_strategy can be BLOCKING, YIELDING, and SPINNING, with increasing
 * CPU resource consumption and decreasing latency.
 *
 * Submit your event hander:
 * disruptor.handleWith(&foo_event_handler);
 *
 * Start your disruptor, which launches the consumer thread inside with your event handler:
 * disruptor.Start();
 *
 * Now you can request slots and publish messages, which will be picked up and handler by
 * the consumers:
 * SequenceRange range = disruptor->Next(3);
 * for (int64 sequence = range.Low(); sequence <= range.High(); sequence ++) {
 *   Foo foo = factory->NewInstance();
 *   foo = ...;
 *   disruptor.Update(sequence, foo);
 * }
 * disruptor.BatchPublish(range);
 *
 * For more examples, please refer to the src/perftest folder.
 */

#include "common.h"
#include "disruptor.h"
#include "event_factory.h"
#include "handlers/event_handler.h"
#include "sequencers/sequence_range.h"

#endif // DISRUPTOR_CPP_INCLUDE_DISRUPTOR_API_H_

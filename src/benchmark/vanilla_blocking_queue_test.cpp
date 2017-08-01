#include "utils/perf_test_helper.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

template<typename T>
class VanillaBlockingQueue {
 public:
	VanillaBlockingQueue(int capacity): capacity(capacity),
    producer_index(0), consumer_index(0), buffer(capacity) {}
  void put(const T& e);
  void put(T&& e);
  T take();

 private:
  bool full() {
    return (producer_index + 1) % capacity == consumer_index
    	    || (consumer_index == -1 && producer_index == capacity - 1 );
  }
  bool empty() {
    return (consumer_index + 1) % capacity == producer_index;
  }
  std::mutex m;
  std::condition_variable producer;
  std::condition_variable consumer;
  int capacity;
  int producer_index;
  int consumer_index;
  std::vector<T> buffer;
};


template<typename T>
void VanillaBlockingQueue<T>::put(const T& e) {
  std::unique_lock<std::mutex> lk(m);
  while (full()) {
    producer.wait(lk);
  }
  buffer[producer_index] = e;
  producer_index ++;
  consumer.notify_one();
}

template<typename T>
void VanillaBlockingQueue<T>::put(T&& e) {
  std::unique_lock<std::mutex> lk(m);
  while (full()) {
    producer.wait(lk);
  }
  buffer[producer_index] = std::move(e);
  producer_index ++;
  consumer.notify_one();
}

template<typename T>
T VanillaBlockingQueue<T>::take() {
  std::unique_lock<std::mutex> lk(m);
  while (empty()) {
    consumer.wait(lk);
  }
  T head = buffer[consumer_index];
  consumer_index ++;
  producer.notify_one();
  return head;
}

void consume(long long iteration, VanillaBlockingQueue<int> *q) {
  long long count = 0;
  do {
    q->take();
    count ++;
  } while (count < iteration);
}

void run(int pass, long long iteration) {
	PerfTestHelper helper;
	VanillaBlockingQueue<int> q(1024);
	std::thread t(consume, iteration, &q);
	helper.Register();
	for (long long i = 0; i < iteration; i ++) {
		q.put(1);
	}
	t.join();
	helper.Finish(pass, iteration);
}

int main() {
	std::cout << "Vanilla Blocking Queue Test" << std::endl;
	int pass = 5;
	long long iteration = 1000 * 10;
	for (int i = 1; i <= pass; i ++) {
		run(i, iteration);
	}
	return 0;
}

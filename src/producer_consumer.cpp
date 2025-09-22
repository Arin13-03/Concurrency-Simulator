#include "producer_consumer.h"
#include "metrics.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <chrono>

using namespace std::chrono_literals;

void producer(std::queue<int> &q, std::mutex &mtx, std::condition_variable &cv, int id, int produceCount) {
    for (int i = 0; i < produceCount; ++i) {
        {
            std::lock_guard<std::mutex> lk(mtx);
            q.push(id * 100 + i);
            incrementMetric("produced");
            std::cout << "[Producer " << id << "] produced " << id*100 + i << "\n";
        }
        cv.notify_one();
        std::this_thread::sleep_for(70ms);
    }
}

void consumer(std::queue<int> &q, std::mutex &mtx, std::condition_variable &cv, int id, int totalToConsume) {
    int consumed = 0;
    while (consumed < totalToConsume) {
        std::unique_lock<std::mutex> lk(mtx);
        cv.wait(lk, [&q]{ return !q.empty(); });
        int val = q.front();
        q.pop();
        ++consumed;
        incrementMetric("consumed");
        std::cout << "  [Consumer " << id << "] consumed " << val << "\n";
    }
}

void runProducerConsumer() {
    std::queue<int> q;
    std::mutex mtx;
    std::condition_variable cv;

    const int numProducers = 2;
    const int numConsumers = 2;
    const int perProducer = 8;

    std::vector<std::thread> producers, consumers;

    for (int i = 0; i < numProducers; ++i)
        producers.emplace_back(producer, std::ref(q), std::ref(mtx), std::ref(cv), i, perProducer);

    int totalItems = numProducers * perProducer;
    int perConsumer = (totalItems + numConsumers - 1) / numConsumers;

    for (int i = 0; i < numConsumers; ++i)
        consumers.emplace_back(consumer, std::ref(q), std::ref(mtx), std::ref(cv), i, perConsumer);

    for (auto &p : producers) p.join();
    cv.notify_all();

    for (auto &c : consumers) c.join();

    std::cout << "Producer-Consumer simulation finished.\n";
}

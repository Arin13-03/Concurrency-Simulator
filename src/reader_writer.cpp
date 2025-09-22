#include "reader_writer.h"
#include "metrics.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <vector>
#include <chrono>

using namespace std::chrono_literals;

static int shared_data = 0;

void reader(int id, std::shared_mutex &rwmutex, int loops) {
    for (int i = 0; i < loops; ++i) {
        std::shared_lock<std::shared_mutex> lk(rwmutex);
        int val = shared_data;
        incrementMetric("reader_ops");
        std::cout << "[Reader " << id << "] read value = " << val << "\n";
        std::this_thread::sleep_for(80ms);
    }
}

void writer(int id, std::shared_mutex &rwmutex, int loops) {
    for (int i = 0; i < loops; ++i) {
        std::unique_lock<std::shared_mutex> lk(rwmutex);
        ++shared_data;
        incrementMetric("writer_ops");
        std::cout << "[Writer " << id << "] incremented value -> " << shared_data << "\n";
        std::this_thread::sleep_for(120ms);
    }
}

void runReaderWriter() {
    std::shared_mutex rwmutex;
    std::vector<std::thread> threads;

    for (int i = 0; i < 3; ++i) threads.emplace_back(reader, i, std::ref(rwmutex), 10);
    for (int i = 0; i < 2; ++i) threads.emplace_back(writer, i, std::ref(rwmutex), 5);

    for (auto &t : threads) t.join();
    std::cout << "Reader-Writer simulation finished.\n";
}

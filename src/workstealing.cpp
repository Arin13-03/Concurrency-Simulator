#include "workstealing.h"
#include "metrics.h"
#include <iostream>
#include <thread>
#include <deque>
#include <mutex>
#include <vector>
#include <random>
#include <chrono>

using namespace std::chrono_literals;

struct Worker {
    std::deque<int> tasks;
    std::mutex m;
};

void worker_run(int id, std::vector<Worker> &workers, std::atomic<bool> &done) {
    std::mt19937 rng(id + 123);
    std::uniform_int_distribution<int> dist(0, workers.size()-1);

    while (!done.load()) {
        int task = -1;
        {
            std::lock_guard<std::mutex> lk(workers[id].m);
            if (!workers[id].tasks.empty()) {
                task = workers[id].tasks.front();
                workers[id].tasks.pop_front();
            }
        }

        if (task != -1) {
            incrementMetric("ws_processed");
            std::this_thread::sleep_for(30ms);
            std::cout << "[W" << id << "] processed task " << task << "\n";
            continue;
        }

        int victim = dist(rng);
        if (victim == id) { std::this_thread::sleep_for(10ms); continue; }
        {
            std::lock_guard<std::mutex> lk(workers[victim].m);
            if (!workers[victim].tasks.empty()) {
                task = workers[victim].tasks.back();
                workers[victim].tasks.pop_back();
            }
        }

        if (task != -1) {
            incrementMetric("ws_stolen");
            std::this_thread::sleep_for(40ms);
            std::cout << "[W" << id << "] stole and processed task " << task << " from W" << victim << "\n";
        } else {
            std::this_thread::sleep_for(10ms);
        }
    }
}

void runWorkStealing() {
    const int numWorkers = 4;
    const int tasksPerWorker = 8;

    std::vector<Worker> workers(numWorkers);
    for (int i = 0; i < numWorkers; ++i) {
        for (int t = 0; t < tasksPerWorker; ++t) {
            workers[i].tasks.push_back(i * 100 + t);
        }
    }

    std::atomic<bool> done{false};
    std::vector<std::thread> pool;
    for (int i = 0; i < numWorkers; ++i)
        pool.emplace_back(worker_run, i, std::ref(workers), std::ref(done));

    std::this_thread::sleep_for(std::chrono::seconds(3));
    done.store(true);

    for (auto &t : pool) t.join();
    std::cout << "Work-Stealing simulation finished.\n";
}

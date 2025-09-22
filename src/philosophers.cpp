#include "philosophers.h"
#include "metrics.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

using namespace std::chrono_literals;

// Each philosopher picks up the lower-numbered fork first to avoid deadlock
void philosopher(int id, std::mutex &left, std::mutex &right, int rounds) {
    for (int i = 0; i < rounds; ++i) {
        std::this_thread::sleep_for(100ms); // Thinking
        // Always lock the lower address first to avoid deadlock
        std::mutex *first = &left, *second = &right;
        if (first > second) std::swap(first, second);
        std::scoped_lock lock(*first, *second);

        incrementMetric("phil_eat_count");
        std::cout << "Philosopher " << id << " is eating (round " << i + 1 << ")\n";
        std::this_thread::sleep_for(150ms); // Eating
    }
}

void runPhilosophers() {
    const int N = 5;
    const int rounds = 5;
    std::vector<std::mutex> forks(N);
    std::vector<std::thread> threads;

    for (int i = 0; i < N; ++i) {
        threads.emplace_back(philosopher, i, std::ref(forks[i]), std::ref(forks[(i+1)%N]), rounds);
    }

    for (auto &t : threads) t.join();
    std::cout << "Dining Philosophers finished.\n";
}

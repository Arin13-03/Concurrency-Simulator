// src/ui.cpp
#include "ui.h"
#include "monitor.h"
#include "metrics.h"
#include "philosophers.h"
#include "reader_writer.h"
#include "producer_consumer.h"
#include "workstealing.h"

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <sstream>
#include <string>

// Minimal interactive UI used when no --mode is passed.
// Keeps things simple: a tiny menu loop to run one simulation (blocking).
// We keep it minimal to avoid another large dependency.

static void printMenu() {
    std::cout << "\nInteractive Concurrency Simulator\n";
    std::cout << "1) Dining Philosophers\n";
    std::cout << "2) Reader-Writer\n";
    std::cout << "3) Producer-Consumer\n";
    std::cout << "4) Work-Stealing Queue\n";
    std::cout << "m) show metrics\n";
    std::cout << "t) toggle monitor printing on/off\n";
    std::cout << "q) quit\n";
    std::cout << "Choose an option: ";
}


void runInteractiveUI() {
    // start monitor thread
    std::thread monitorThread(monitorSystem);

    while (true) {
        printMenu();
        std::string input;
        if (!std::getline(std::cin, input)) break;
        if (input.empty()) continue;

        if (input == "q" || input == "Q") {
            std::cout << "Quitting interactive UI...\n";
            break;
        } else if (input == "m" || input == "M") {
            std::cout << getMetricsSnapshot() << std::endl;
            continue;
        } else if (input == "t" || input == "T") {
            // toggle monitor printing
            static bool enabled = true;
            enabled = !enabled;
            enableMonitorPrinting(enabled);
            std::cout << "Monitor printing " << (enabled ? "enabled" : "disabled") << "\n";
            continue;
        }

        int choice = 0;
        try {
            choice = std::stoi(input);
        } catch (...) {
            std::cout << "Invalid input.\n";
            continue;
        }

        switch (choice) {
            case 1:
                runPhilosophers();
                break;
            case 2:
                runReaderWriter();
                break;
            case 3:
                runProducerConsumer();
                break;
            case 4:
                runWorkStealing();
                break;
            default:
                std::cout << "Invalid choice.\n";
        }
    }

    // stop monitor and join
    stopMonitor();
    if (monitorThread.joinable()) monitorThread.join();
}


// #include <iostream>
// #include <thread>
// #include "monitor.h"
// #include "philosophers.h"
// #include "reader_writer.h"
// #include "producer_consumer.h"
// #include "workstealing.h"

// using namespace std;

// int main() {
//     cout << "Process Synchronization Simulator\n";
//     cout << "1. Dining Philosophers\n2. Reader-Writer\n3. Producer-Consumer\n4. Work-Stealing Queue\n";
//     cout << "Choose (1-4): ";

//     int choice; 
//     if(!(cin >> choice)) {
//         cout << "Invalid input. Exiting.\n";
//         return 1;
//     }

//     std::thread monitorThread(monitorSystem);

//     switch (choice) {
//         case 1: runPhilosophers(); break;
//         case 2: runReaderWriter(); break;
//         case 3: runProducerConsumer(); break;
//         case 4: runWorkStealing(); break;
//         default: cout << "Invalid choice\n";
//     }

//     stopMonitor();
//     monitorThread.join();

//     return 0;
// }


// src/main.cpp
#include <iostream>
#include <string>
#include <cstring>
#include <thread>    // <<--- ADD THIS
#include <cstdlib>   // for std::atoi

// forward declarations
void runInteractiveUI();
void runPhilosophers();
void runReaderWriter();
void runProducerConsumer();
void runWorkStealing();
void monitorSystem();
void stopMonitor();

int runMode(int mode) {
    // start monitor thread
    std::thread mon(monitorSystem);   // now std::thread is available

    switch (mode) {
        case 1: runPhilosophers(); break;
        case 2: runReaderWriter(); break;
        case 3: runProducerConsumer(); break;
        case 4: runWorkStealing(); break;
        default:
            std::cerr << "Invalid mode " << mode << "\n";
            stopMonitor();
            mon.join();
            return 1;
    }

    // stop monitor and join
    stopMonitor();
    mon.join();
    return 0;
}

int main(int argc, char **argv) {
    // check args for --mode N
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--mode") == 0 && i+1 < argc) {
            int mode = std::atoi(argv[i+1]);
            return runMode(mode);
        }
    }

    // if no --mode, fallback to interactive UI
    runInteractiveUI();
    return 0;
}

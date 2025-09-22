// #include "monitor.h"
// #include "metrics.h"
// #include <atomic>
// #include <iostream>
// #include <thread>
// #include <chrono>

// static std::atomic<bool> monitor_running{true};

// void monitorSystem() {
//     using namespace std::chrono_literals;
//     while (monitor_running.load()) {
//         std::this_thread::sleep_for(1s);
//         auto snapshot = getMetricsSnapshot();
//         std::cout << "[Monitor] " << snapshot << std::endl;
//     }
//     std::cout << "[Monitor] Exiting monitor thread.\n";
// }

// void stopMonitor() {
//     monitor_running.store(false);
// }

// src/monitor.cpp
#include "monitor.h"
#include "metrics.h"

#include <atomic>
#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>
#include <fstream>
#include <string>
#include <cctype>

static std::atomic<bool> monitor_running{true};
static std::atomic<bool> monitor_printing{true};

static std::string trim(std::string s) {
    while (!s.empty() && std::isspace((unsigned char)s.front())) s.erase(s.begin());
    while (!s.empty() && std::isspace((unsigned char)s.back())) s.pop_back();
    return s;
}

void enableMonitorPrinting(bool on) {
    monitor_printing.store(on);
}

void monitorSystem() {
    using namespace std::chrono;
    std::string last_snapshot;
    while (monitor_running.load()) {
        std::this_thread::sleep_for(seconds(1));

        auto snapshot = getMetricsSnapshot(); // e.g. "Metrics{a:1, b:2}"

        // Print only when changed AND printing is enabled
        if (monitor_printing.load() && snapshot != last_snapshot) {
            std::cout << "[Monitor] " << snapshot << std::endl;
            last_snapshot = snapshot;
        }

        // Write metrics.json for the web UI (if web folder exists)
        std::string json_path = std::string("web/metrics.json");
        try {
            std::ofstream ofs(json_path, std::ofstream::trunc);
            if (!ofs) continue;

            auto l = snapshot.find('{');
            auto r = snapshot.rfind('}');
            ofs << "{\n";
            if (l != std::string::npos && r != std::string::npos && r > l) {
                std::string body = snapshot.substr(l + 1, r - l - 1);
                std::istringstream iss(body);
                std::string token;
                bool first = true;
                while (std::getline(iss, token, ',')) {
                    auto colon = token.find(':');
                    if (colon == std::string::npos) continue;
                    std::string key = trim(token.substr(0, colon));
                    std::string val = trim(token.substr(colon + 1));
                    if (!first) ofs << ",\n";
                    ofs << "  \"" << key << "\": " << val;
                    first = false;
                }
            }
            ofs << "\n}\n";
            ofs.close();
        } catch (...) {
            // ignore file I/O errors
        }
    }
    std::cout << "[Monitor] Exiting monitor thread.\n";
}

void stopMonitor() {
    monitor_running.store(false);
}


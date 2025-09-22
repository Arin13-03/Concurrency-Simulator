#include "metrics.h"
#include <map>
#include <mutex>
#include <sstream>

static std::map<std::string, long> metrics;
static std::mutex metrics_mtx;

void incrementMetric(const std::string &name, long delta) {
    std::lock_guard<std::mutex> lk(metrics_mtx);
    metrics[name] += delta;
}

std::string getMetricsSnapshot() {
    std::lock_guard<std::mutex> lk(metrics_mtx);
    std::ostringstream oss;
    oss << "Metrics{";
    bool first = true;
    for (auto &p : metrics) {
        if (!first) oss << ", ";
        oss << p.first << ":" << p.second;
        first = false;
    }
    oss << "}";
    return oss.str();
}

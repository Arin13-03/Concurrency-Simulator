#ifndef METRICS_H
#define METRICS_H

#include <string>

void incrementMetric(const std::string &name, long delta = 1);
std::string getMetricsSnapshot();

#endif // METRICS_H

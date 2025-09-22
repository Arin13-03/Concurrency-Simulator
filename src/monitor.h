#ifndef MONITOR_H
#define MONITOR_H

// monitor provides a simple system monitor thread
// monitorSystem() runs until stopMonitor() is called.

void monitorSystem();
void stopMonitor();

// Enable or disable monitor printing to stdout at runtime.
// The monitor thread will still run and update metrics.json.
void enableMonitorPrinting(bool on);

#endif // MONITOR_H

# Concurrency Simulator

A simple yet educational process synchronization simulator that demonstrates classic concurrency problems and their solutions. This project is ideal for students, educators, and anyone interested in understanding how threads and synchronization primitives work in practice.

## Features

- **Dining Philosophers**: Simulates philosophers sharing forks, illustrating deadlock avoidance and resource ordering.
- **Reader-Writer**: Models concurrent access to shared data, demonstrating reader/writer lock strategies.
- **Producer-Consumer**: Shows how producers and consumers interact with a bounded buffer using condition variables.
- **Work-Stealing Queue**: Demonstrates dynamic load balancing among worker threads.

## How It Works

Each simulation runs as a separate job, using C++ threads and synchronization primitives (mutexes, condition variables, etc.). The simulator can be run from the command line or integrated with a web UI for interactive exploration.

---

## Build Instructions

### Prerequisites

- **C++17 or newer** compiler (g++, clang++, or MSVC)
- **CMake** (for cross-platform builds)
- **pthread** library (on Linux/macOS/MinGW)

### Build with CMake (Recommended)

```bash
git clone https://github.com/Arin13-03/Concurrency-Simulator.git
cd concurrency-simulator
mkdir build
cd build
cmake ..
cmake --build .
./ConcurrencySimulator
```

### Manual Compilation

If you prefer not to use CMake, compile all source files directly:

```bash
g++ -std=c++17 -pthread src/*.cpp -o concurrency_simulator
./concurrency_simulator
```

---

## Running the Simulator

### Command Line

After building, launch the executable:

```bash
./ConcurrencySimulator
```

You will be prompted to select a simulation mode:

```
Select simulation mode:
1. Dining Philosophers
2. Reader-Writer
3. Producer-Consumer
4. Work-Stealing Queue
Enter choice [1-4]:
```

Enter the number corresponding to the simulation you want to run. The program will then execute the selected scenario, printing output and metrics to the console.

---

## Simulation Modes Explained

### 1. Dining Philosophers

- **Problem**: Five philosophers sit at a table with five forks. Each needs two forks to eat. If all pick up the left fork first, a deadlock occurs.
- **Solution**: Philosophers always pick up the lower-numbered fork first, preventing circular wait and deadlock.

### 2. Reader-Writer

- **Problem**: Multiple readers can read shared data, but writers need exclusive access.
- **Solution**: Uses reader/writer locks to allow concurrent reads but exclusive writes.

### 3. Producer-Consumer

- **Problem**: Producers generate data and put it in a bounded buffer; consumers remove data. Synchronization is needed to avoid buffer overflows/underflows.
- **Solution**: Uses mutexes and condition variables to coordinate access to the buffer.

### 4. Work-Stealing Queue

- **Problem**: Multiple worker threads process tasks. Idle workers can "steal" tasks from others to balance the load.
- **Solution**: Implements a work-stealing queue for dynamic load balancing.

---

## Demo 
<img width="987" height="657" alt="Image" src="https://github.com/user-attachments/assets/1dbc275b-bb28-4e8b-a643-c14d64e03f7b" />

---

## Project Structure

```
concurrency-simulator/
├── src/                # C++ source code for all simulators and metrics
│   ├── philosophers.cpp
│   ├── metrics.cpp
│   └── ...
├── web/                # (Optional) Web UI and backend
│   ├── static/
│   │   └── index.html  # Main web UI
│   └── app.py          # Flask backend
├── build/              # Build output (created after compilation)
├── README.md
└── CMakeLists.txt
```

---

## Extending the Simulator

- **Add new problems:** Implement a new C++ file in `src/` and register it in the main entry point.
- **Add metrics:** Use `incrementMetric()` in your C++ code to track custom metrics.
- **Customize UI:** Edit `web/static/index.html` for new features or visual styles.

---

## Troubleshooting

- **Build errors:** Ensure your compiler supports C++17 or newer and that all dependencies are installed.
- **Permission errors:** Run the simulator with appropriate permissions to write logs and metrics.
- **Web UI not updating:** If using the web UI, ensure the backend (`app.py`) is running and accessible.

---


## Credits

- Inspired by classic operating systems and concurrency textbooks.
- Web UI built with Flask and vanilla JavaScript/CSS.

---


Enjoy learning about concurrency!





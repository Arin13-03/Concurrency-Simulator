cmake_minimum_required(VERSION 3.10)
project(ConcurrencySimulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

file(GLOB SRC "src/*.cpp")

add_executable(ConcurrencySimulator ${SRC})
target_include_directories(ConcurrencySimulator PRIVATE src)
target_link_libraries(ConcurrencySimulator PRIVATE Threads::Threads)

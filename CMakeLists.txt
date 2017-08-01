cmake_minimum_required (VERSION 3.0)

project(disruptor-cpp)
set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11 -O2")

find_package(Boost 1.40 REQUIRED)
include_directories(include ${Boost_INCLUDE_DIR})

#file(GLOB SOURCES "src/*.cpp" "src/*.h") 

add_executable(boost_lock_free_queue_test src/benchmark/boost_lock_free_queue_test.cpp)
add_executable(vanilla_blocking_queue_test src/benchmark/vanilla_blocking_queue_test.cpp)
add_executable(boost_wait_free_queue_test src/benchmark/boost_wait_free_queue_test.cpp)
add_executable(disruptor_one_to_one_test src/perftest/disruptor_one_to_one_test.cpp src/perftest/test_setup.h)
add_executable(disruptor_one_to_many_test src/perftest/disruptor_one_to_many_test.cpp src/perftest/test_setup.h)
add_executable(disruptor_many_to_many_test src/perftest/disruptor_many_to_many_test.cpp src/perftest/test_setup.h)


#target_link_libraries(one_to_one_lock_free_queue_test LINK_PUBLIC ${Boost_LIBRARIES})

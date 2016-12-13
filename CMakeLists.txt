cmake_minimum_required(VERSION 3.6)
project(calc_pi_mc)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -lm -static-libgcc -lquadmath -lgfortran -std=c++11 ")
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.c aux.c aux.h kalman.c kalman.h main.h rng.c rng.h)

add_executable(calc_pi_mc ${SOURCE_FILES})

target_link_libraries(calc_pi_mc m Threads::Threads)




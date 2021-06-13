cmake_minimum_required(VERSION 3.10)

project(dbmsProj)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(src/include)
add_subdirectory(src/libs)
add_subdirectory(src/deps)
set(EXEC "${PROJECT_NAME}.out")
add_executable("${EXEC}" src/main.cpp)

target_link_libraries(${EXEC} PUBLIC dbms PUBLIC Threads::Threads)
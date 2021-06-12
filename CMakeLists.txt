cmake_minimum_required(VERSION 3.10)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")

project(dbmsProj)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(src/include)
add_subdirectory(src/libs)
set(EXEC "${PROJECT_NAME}.out")
add_executable("${EXEC}" src/main.cpp)

target_link_libraries(${EXEC} PUBLIC dbms PUBLIC ui PUBLIC Threads::Threads)
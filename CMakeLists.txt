cmake_minimum_required(VERSION 3.10)

project(dbmsProj)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package( Boost 1.40 COMPONENTS program_options REQUIRED )
set(wxWidgets_CONFIGURATION mswu)
find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(src/include ${Boost_INCLUDE_DIR} )

add_subdirectory(src/libs)
set(EXEC "${PROJECT_NAME}.out")
add_executable("${EXEC}" src/main.cpp)

target_link_libraries(${EXEC} PUBLIC dbms PUBLIC ui PUBLIC ${Boost_LIBRARIES} PUBLIC Threads::Threads PUBLIC ${wxWidgets_LIBRARIES})


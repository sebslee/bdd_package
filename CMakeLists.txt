cmake_minimum_required(VERSION 2.8)
project(VDSProject CXX C)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11 ${LLVM_CXX_FLAGS} ")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#Include dirs
include_directories(${CMAKE_SOURCE_DIR}/src)
link_directories(${CMAKE_SOURCE_DIR}/src)

include_directories(${CMAKE_SOURCE_DIR}/src/test/)
link_directories(${CMAKE_SOURCE_DIR}/src/test/)

include_directories(${CMAKE_SOURCE_DIR}/src/bench/)
link_directories(${CMAKE_SOURCE_DIR}/src/bench/)

add_definitions (-D_GLIBCXX_USE_CXX11_ABI=0)

add_subdirectory(src)
add_executable(VDSProject src/main.cpp)
target_link_libraries(VDSProject Manager)

#find_package(Boost COMPONENTS system filesystem REQUIRED)

#add_executable(VDSProject_bench main_bench.cpp)
#target_link_libraries(VDSProject_bench Manager)
#target_link_libraries(VDSProject_bench Benchmark)
#target_link_libraries(VDSProject_bench ${Boost_LIBRARIES})

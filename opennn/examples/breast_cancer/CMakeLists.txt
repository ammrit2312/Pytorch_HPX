# Specify the minimum version for CMake
cmake_minimum_required(VERSION 2.8.10)

# Project's name
project(breast_cancer)
find_package(HPX REQUIRED)

link_directories(${CMAKE_SOURCE_DIR}/opennn)

include_directories(${CMAKE_SOURCE_DIR}/opennn)

#add_executable(breast_cancer main.cpp)
add_hpx_executable(breast_cancer
	ESSENTIAL
        SOURCES main.cpp
        COMPONENT_DEPENDENCIES iostreams)

target_link_libraries(breast_cancer opennn)



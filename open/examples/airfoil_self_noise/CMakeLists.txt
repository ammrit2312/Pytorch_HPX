# Specify the minimum version for CMake
cmake_minimum_required(VERSION 2.8.10)

# Project's name
project(airfoil_self_noise)
find_package(HPX REQUIRED)

link_directories(${CMAKE_SOURCE_DIR}/opennn)

include_directories(${CMAKE_SOURCE_DIR}/opennn)

#add_executable(airfoil_self_noise main.cpp)
add_hpx_executable(airfoil_self_noise
	ESSENTIAL
        SOURCES main.cpp
        COMPONENT_DEPENDENCIES iostreams)

target_link_libraries(airfoil_self_noise opennn)



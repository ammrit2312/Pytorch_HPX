# Require a recent version of cmake
cmake_minimum_required(VERSION 2.8)

# This project is C++ based.
project(adaptive_moment_estimation CXX)

# Instruct cmake to find the HPX settings
find_package(HPX REQUIRED)

# Build your application using HPX
add_hpx_library(adaptive_moment_estimation
       ESSENTIAL
       SOURCES adaptive_moment_estimation.cpp
       DEPENDENCIES ${Torch_LIBS}
       COMPONENT_DEPENDENCIES iostreams)

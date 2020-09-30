# Require a recent version of cmake
cmake_minimum_required(VERSION 2.8)

# This project is C++ based.
project(autograd CXX)

# Instruct cmake to find the HPX settings
#Change HPXYMY to HPX
find_package(HPX REQUIRED)

find_package(Torch REQUIRED PATHS /Users/amriteshamrit/pytorch)

# Build your application using HPX
add_hpx_executable(autograd
        ESSENTIAL
        SOURCES hpx_autograd.cpp
        DEPENDENCIES ${Torch_LIBS}
        COMPONENT_DEPENDENCIES iostreams)

target_link_libraries(${PROJECT_NAME} "${TORCH_LIBRARIES}")

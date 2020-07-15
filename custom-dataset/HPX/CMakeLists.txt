# Require a recent version of cmake
cmake_minimum_required(VERSION 2.8)

# This project is C++ based.
project(custom-dataset CXX)
set(CMAKE_CXX_STANDARD 17)

# Instruct cmake to find the HPX settings
find_package(HPX REQUIRED)
find_package(Torch REQUIRED PATHS /Users/amriteshamrit/pytorch)
find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs)

# Build your application using HPX
add_hpx_executable(custom-dataset
        ESSENTIAL
        SOURCES hpx_custom-dataset.cpp
        DEPENDENCIES ${Torch_LIBS}
        COMPONENT_DEPENDENCIES iostreams)

target_link_libraries(${PROJECT_NAME} "${OpenCV_LIBS}")
target_link_libraries(${PROJECT_NAME} "${TORCH_LIBRARIES}")

configure_file("info.txt" "info.txt" COPYONLY)

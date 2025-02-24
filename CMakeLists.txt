cmake_minimum_required (VERSION 3.6 FATAL_ERROR)

# Project name
project(LearnOpenCASCADE CXX)

# Enable automatic MOC, UIC, and RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON) 

# Set the directory where OpenCASCADE is installed
set(OpenCASCADE_DIR "E:/Sources/OCCT-7_9_0/out/install/x64-Debug/cmake")

# OpenCascade
find_package(OpenCASCADE) 

# Configure C++ compiler's includes dir
include_directories(SYSTEM ${OpenCASCADE_INCLUDE_DIR} )

# Add linker options
foreach (LIB ${OpenCASCADE_LIBRARIES})
  link_libraries( debug ${OpenCASCADE_LIBRARY_DIR}d/${LIB}.lib)
  link_libraries( optimized ${OpenCASCADE_LIBRARY_DIR}/${LIB}.lib)
endforeach()

message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(lessons)
cmake_minimum_required (VERSION 3.6 FATAL_ERROR)

# Project name
project(LearnOpenCASCADE CXX)

# Enable automatic MOC, UIC, and RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON) 

# OpenCascade
find_package(OpenCASCADE REQUIRED) 

# Configure C++ compiler's includes dir
include_directories(SYSTEM ${OpenCASCADE_INCLUDE_DIR} )

# Add linker options
foreach (LIB ${OpenCASCADE_LIBRARIES})
  link_libraries( debug ${OpenCASCADE_LIBRARY_DIR}d/${LIB}.lib)
  link_libraries( optimized ${OpenCASCADE_LIBRARY_DIR}/${LIB}.lib)
endforeach()

add_subdirectory(lessons)

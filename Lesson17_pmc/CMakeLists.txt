cmake_minimum_required (VERSION 3.12.0 FATAL_ERROR)

# Project name
project (Lesson_17_pmc CXX)

# Set the directory where OpenCASCADE is installed
set(OpenCASCADE_DIR "C:/Dependencies/occt-7.8-vc14-64/cmake")
set(VTK_DIR "C:/Dependencies/VTK-6.1.0-vc14-64/lib/cmake/vtk-6.1")

# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# OpenCascade
find_package(OpenCASCADE)

# Configure C++ compiler's includes dir
include_directories ( SYSTEM ${OpenCASCADE_INCLUDE_DIR} )

# Add executable
add_executable (Lesson_17_pmc
  ClassifyPt.h
  main.cpp
  Viewer.cpp
  Viewer.h
  ViewerInteractor.cpp
  ViewerInteractor.h
)

# Add linker options
foreach (LIB ${OpenCASCADE_LIBRARIES})
  target_link_libraries(Lesson_17_pmc debug ${OpenCASCADE_LIBRARY_DIR}d/${LIB}.lib)
  target_link_libraries(Lesson_17_pmc optimized ${OpenCASCADE_LIBRARY_DIR}/${LIB}.lib)
endforeach()

# Adjust runtime environment
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT Lesson_17_pmc)
set_property(TARGET Lesson_17_pmc PROPERTY VS_DEBUGGER_ENVIRONMENT "PATH=$<$<CONFIG:DEBUG>:${OpenCASCADE_BINARY_DIR}d>$<$<NOT:$<CONFIG:DEBUG>>:${OpenCASCADE_BINARY_DIR}>;%PATH%")

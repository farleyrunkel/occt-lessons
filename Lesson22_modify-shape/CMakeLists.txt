cmake_minimum_required (VERSION 3.9.0 FATAL_ERROR)

# Project name
project (Lesson_22_modify-shape CXX)
# Set the directory where OpenCASCADE is installed
set(OpenCASCADE_DIR "C:/Dependencies/occt-7.8-vc14-64/cmake")

# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# OpenCascade
find_package(OpenCASCADE)

# Configure C++ compiler's includes dir
include_directories ( SYSTEM ${OpenCASCADE_INCLUDE_DIR} )

# Add executable
add_executable (Lesson_22_modify-shape
  main.cpp
  Viewer.cpp
  Viewer.h
  ViewerInteractor.cpp
  ViewerInteractor.h
)

# Add linker options
foreach (LIB ${OpenCASCADE_LIBRARIES})
  target_link_libraries(Lesson_22_modify-shape debug ${OpenCASCADE_LIBRARY_DIR}d/${LIB}.lib)
  target_link_libraries(Lesson_22_modify-shape optimized ${OpenCASCADE_LIBRARY_DIR}/${LIB}.lib)
endforeach()

# Adjust runtime environment
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT Lesson_22_modify-shape)
set_property(TARGET Lesson_22_modify-shape PROPERTY VS_DEBUGGER_ENVIRONMENT "PATH=$<$<CONFIG:DEBUG>:${OpenCASCADE_BINARY_DIR}d>$<$<NOT:$<CONFIG:DEBUG>>:${OpenCASCADE_BINARY_DIR}>;%PATH%")

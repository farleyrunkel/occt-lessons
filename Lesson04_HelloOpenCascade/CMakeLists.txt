cmake_minimum_required (VERSION 3.0.0 FATAL_ERROR)

# Project name
project (Lesson_04 CXX)

# Set the directory where OpenCASCADE is installed
set(OpenCASCADE_DIR "C:/Dependencies/occt-7.8-vc14-64/cmake")
set(VTK_DIR "C:/Dependencies/VTK-6.1.0-vc14-64/lib/cmake/vtk-6.1")

# OpenCascade
find_package(OpenCASCADE)

# Check if OpenCASCADE is found
if(OpenCASCADE_FOUND)
    message(STATUS "OpenCASCADE found!")
    message(STATUS "OpenCASCADE config file found at: ${OpenCASCADE_CONFIG}")    
    message(STATUS "OpenCASCADE_INCLUDE_DIRS: ${OpenCASCADE_INCLUDE_DIRS}")
    message(STATUS "OpenCASCADE_LIBRARIES: ${OpenCASCADE_LIBRARIES}")
else()
    # If OpenCASCADE is not found, print error message and terminate CMake configuration
    message(FATAL_ERROR "OpenCASCADE not found!")
endif()
# Qt
find_package(Qt)

# Configure C++ compiler's includes dir
include_directories ( SYSTEM ${OpenCASCADE_INCLUDE_DIR} )

# Add executable
add_executable (Lesson_04
  main.cpp
  Viewer.cpp
  Viewer.h
  ViewerInteractor.cpp
  ViewerInteractor.h
)

# Add linker options
foreach (LIB ${OpenCASCADE_LIBRARIES})
  target_link_libraries(Lesson_04 debug ${OpenCASCADE_LIBRARY_DIR}d/${LIB}.lib)
  target_link_libraries(Lesson_04 optimized ${OpenCASCADE_LIBRARY_DIR}/${LIB}.lib)
endforeach()

# Adjust runtime environment
set_property(TARGET Lesson_04 PROPERTY VS_DEBUGGER_ENVIRONMENT "PATH=$<$<CONFIG:DEBUG>:${OpenCASCADE_BINARY_DIR}d>$<$<NOT:$<CONFIG:DEBUG>>:${OpenCASCADE_BINARY_DIR}>;%PATH%")

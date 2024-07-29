cmake_minimum_required (VERSION 3.0.0 FATAL_ERROR)

# Project name
project (Lesson_10_OcafEx CXX)

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

# Configure C++ compiler's includes dir
include_directories ( SYSTEM ${OpenCASCADE_INCLUDE_DIR} )

# Add executable
add_executable (Lesson_10_OcafEx
  src/main.cpp
)

message (STATUS "OpenCASCADE_LIBRARIES: ${OpenCASCADE_LIBRARIES}")

# Add linker options
foreach (LIB ${OpenCASCADE_LIBRARIES})
  if (MSVC)
    target_link_libraries(Lesson_10_OcafEx debug ${OpenCASCADE_LIBRARY_DIR}d/${LIB}.lib)
    target_link_libraries(Lesson_10_OcafEx optimized ${OpenCASCADE_LIBRARY_DIR}/${LIB}.lib)
  else()
    target_link_libraries(Lesson_10_OcafEx optimized ${OpenCASCADE_LIBRARY_DIR}/lib${LIB}.so)
  endif()
endforeach()

# Adjust runtime environment
set_property(TARGET Lesson_10_OcafEx PROPERTY VS_DEBUGGER_ENVIRONMENT "PATH=$<$<CONFIG:DEBUG>:${OpenCASCADE_BINARY_DIR}d>$<$<NOT:$<CONFIG:DEBUG>>:${OpenCASCADE_BINARY_DIR}>;%PATH%")

install (TARGETS Lesson_10_OcafEx CONFIGURATIONS Release RUNTIME DESTINATION bin LIBRARY DESTINATION bin COMPONENT Runtime)

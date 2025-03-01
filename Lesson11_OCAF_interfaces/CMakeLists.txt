cmake_minimum_required (VERSION 3.0.0 FATAL_ERROR)

# Project name
project (Lesson_11_OcafEx CXX)

# OpenCascade
find_package(OpenCASCADE)

# Configure C++ compiler's includes dir
include_directories ( SYSTEM ${OpenCASCADE_INCLUDE_DIR} )

# Add executable
add_executable (Lesson_11_OcafEx
  src/main.cpp
)

#message(STATUS ${OpenCASCADE_LIBRARIES})

# Add linker options
foreach (LIB ${OpenCASCADE_LIBRARIES})
  if (WIN32)
    target_link_libraries(Lesson_11_OcafEx debug ${OpenCASCADE_LIBRARY_DIR}d/${LIB}.lib)
    target_link_libraries(Lesson_11_OcafEx optimized ${OpenCASCADE_LIBRARY_DIR}/${LIB}.lib)
  else()
    target_link_libraries(Lesson_11_OcafEx debug ${OpenCASCADE_LIBRARY_DIR}d/lib${LIB}.so)
    target_link_libraries(Lesson_11_OcafEx optimized ${OpenCASCADE_LIBRARY_DIR}/lib${LIB}.so)
  endif()
endforeach()

# Adjust runtime environment
set_property(TARGET Lesson_11_OcafEx PROPERTY VS_DEBUGGER_ENVIRONMENT "PATH=$<$<CONFIG:DEBUG>:${OpenCASCADE_BINARY_DIR}d>$<$<NOT:$<CONFIG:DEBUG>>:${OpenCASCADE_BINARY_DIR}>;%PATH%")

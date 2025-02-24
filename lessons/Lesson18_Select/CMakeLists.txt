
####################
# Qt
####################

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Test OpenGLWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Core5Compat Test OpenGLWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS 3DCore 3DExtras 3DInput 3DRender)

####################
# OpenGL
####################

# Find OpenGL package
find_package(OpenGL REQUIRED)
if(OpenGL_FOUND)
	message(STATUS "OpenGL found")
	message(STATUS "OpenGL include directory: ${OPENGL_INCLUDE_DIR}")
	message(STATUS "OpenGL libraries: ${OPENGL_LIBRARIES}")
else()
	message(FATAL_ERROR "OpenGL not found")
endif()


set(taget_name "Lesson18_Select")

# Add executable
add_executable (${taget_name} 
"main.cpp" "OcctGlTools.cpp" "OcctGlTools.h"
"OcctQtViewer.cpp" "OcctQtViewer.h" "mainwindow.h")
 
# Link Qt and OpenGL libraries
target_link_libraries(${taget_name}  PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Test )
target_link_libraries(${taget_name}  PRIVATE Qt${QT_VERSION_MAJOR}::3DCore Qt${QT_VERSION_MAJOR}::3DExtras Qt${QT_VERSION_MAJOR}::3DInput Qt${QT_VERSION_MAJOR}::3DRender)
target_link_libraries(${taget_name}  PRIVATE Qt${QT_VERSION_MAJOR}::OpenGLWidgets)
target_link_libraries(${taget_name}  PRIVATE Qt${QT_VERSION_MAJOR}::Core5Compat)
target_link_libraries(${taget_name}  PRIVATE ${OPENGL_LIBRARIES})
target_link_libraries(${taget_name}  PRIVATE ${OPENGL_LIBRARIES})


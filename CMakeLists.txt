cmake_minimum_required(VERSION 3.0.2)

project(noiseless)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Setting the compiler data
SET(CMAKE_MACRO_DIRS "${PROJECT_SOURCE_DIR}/cmake/")
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/;${CMAKE_MODULE_PATH}")
link_directories(${CMAKE_BINARY_DIR} "/usr/local/lib/")

# Setting the folder data
set(SOURCES "${PROJECT_SOURCE_DIR}/src/")
set(TEST "${PROJECT_SOURCE_DIR}/test/")
set(PROGRAM "${PROJECT_SOURCE_DIR}/program/")

# Setting the include and external includes and data
IF(WIN32)
  set(EXTERNAL_LIBS "${PROJECT_SOURCE_DIR}/external/windows")
  set(EXTERNAL_INCLUDES "${PROJECT_SOURCE_DIR}/include/windows")
ENDIF()

include(${CMAKE_MACRO_DIRS}/Macros.cmake)

IF(WIN32)
  add_definitions( -DWINDOWSPC )
  FIND_LIBRARY(OpenGL_LIBRARY OpenGL )
  FIND_PACKAGE(OpenCL REQUIRED)
  set(GRAPHIC_INCLUDE_DIR "${GRAPHIC_INCLUDE_DIR}" "${OPENGL_INCLUDE_DIRS}" "${OpenCL_INCLUDE_DIRS}")
  SET(GRAPHIC_LIBS "${GRAPHIC_LIBS}" "${EXTERNAL_LIBS}/lib-vc2015/glfw3.lib" "${EXTERNAL_LIBS}/glew32.lib" "${EXTERNAL_LIBS}/glew32s.lib" "opengl32.lib" "${OpenCL_LIBRARIES}")
  SET(CORE_LIBS ${GRAPHIC_LIBS} )
ENDIF()

# Generating the libraries
GENERATE_LIB("${SOURCES}/base" base "noiseless")
GENERATE_LIB("${SOURCES}/containers" containers "noiseless")
GENERATE_LIB("${SOURCES}/memory" memory "noiseless")
GENERATE_LIB("${SOURCES}/math" math "noiseless")
GENERATE_LIB("${SOURCES}/core" core "noiseless")

SET(CORE_LIBS ${CORE_LIBS} base containers memory math core)

# Adding the tests if we should
add_subdirectory(${TEST})

# Adding the programs if we should
#add_subdirectory(${PROGRAM})
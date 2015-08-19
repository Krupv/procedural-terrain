cmake_minimum_required (VERSION 2.8)

project ("terrain" CXX C)


message (${CMAKE_SYSTEM_NAME})
#CXXFLAGS
set(DEFAULT_FLAGS "-pipe -std=c++14 -DGLEW")

set(CLANG_DEBUG_FLAGS "-DDEBUG -ggdb3 -fcolor-diagnostics -fdiagnostics-format=vi")
set(CLANG_RELEASE_FLAGS "-O2 -fomit-frame-pointer -fvisibility-inlines-hidden")
set(GCC_DEBUG_FLAGS "-Og -DDEBUG -ggdb3 -fdiagnostics-color=always")
set(GCC_RELEASE_FLAGS "-O3 -fomit-frame-pointer -fvisibility-inlines-hidden -fdiagnostics-color=always")

set(WARNINGS " -Wall -Wextra -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable -Wno-ignored-qualifiers")
set(WIN_WARNINGS "/W4 /WX /EHsc")


#BUILD TYPE
#TEMP
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release")
endif()

message(STATUS "Using target: " ${CMAKE_BUILD_TYPE})

if (CMAKE_BUILD_TYPE MATCHES "Debug")
	add_definitions(-DDEBUG=1)
endif()

#COMPILER
if (CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS_DEBUG "${GCC_DEBUG_FLAGS} ${DEFAULT_FLAGS} ${aWARNINGS}")
	set(CMAKE_CXX_FLAGS_RELEASE "${GCC_RELEASE_FLAGS} ${DEFAULT_FLAGS} ${aWARNINGS}")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELEASE} -g")
	message(STATUS "Gnu-Compiler detected, setting custom flags")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMAKE_CXX_FLAGS_DEBUG "${DEFAULT_FLAGS} ${aWARNINGS} ${CLANG_DEBUG_FLAGS} -ferror-limit=10")
	set(CMAKE_CXX_FLAGS_RELEASE "${DEFAULT_FLAGS} ${aWARNINGS} ${CLANG_RELEASE_FLAGS}")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELEASE} -g")
	message(STATUS "Clang-Compiler detected, setting custom flags")
endif()


set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/modules)

include(${CMAKE_MODULE_PATH}/FindGLEW.cmake)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
link_directories(${OPENGL_LIBRARY_DIRS})
add_definitions(${OPENGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
	message(Error "OpenGL not found")
endif(NOT OPENGL_FOUND)

#GLM
find_path(GLM_INCLUDE_DIRS "glm/glm.hpp"
	HINTS
	"${GLM_DIR}/include"
	"${CMAKE_SOURCE_DIR}/include"
	)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
	GLM
	DEFAULT_MSG
	GLM_INCLUDE_DIRS
	)
if(NOT GLM_FOUND)
	message(Error "GLM not found")
else()
	include_directories(${GLM_INCLUDE_DIRS})
endif(NOT GLM_FOUND)

################################################################################
#SFML
########################################

find_package(SFML 2 COMPONENTS graphics window system  REQUIRED)
########################################
################################################################################



set (CORE_F "core")
set (REND_F "rendering")
set (GEN_F "generator")
set (STA_F "static_libs")

#CHANGE FOR NOISE GENERATOP
if (NOT VALUE_NOISE)
	message("using perlin noise")
	add_definitions(-DPERLIN)
endif (NOT VALUE_NOISE)

set (RESOURCE_FOLDER "resources")
add_definitions(-DRESOURCE_FOLDER="resources/")


# Lets LOAD app our headers!
file(GLOB_RECURSE HDRS
	${terrain_SOURCE_DIR}/src/*.hpp
	${terrain_SOURCE_DIR}/src/*.h
)

# Lets LOAD app our sources!
file(GLOB_RECURSE SRCS
	${terrain_SOURCE_DIR}/src/*.c
	${terrain_SOURCE_DIR}/src/*.cpp
)

add_executable(terrain ${HDRS} ${SRCS})

include_directories(
	${GLEW_INCLUDE_DIR}
	${SFML_INCLUDE_DIR}
)

target_link_libraries(terrain
	${GLEW_LIBRARIES}
	${OPENGL_LIBRARIES}
	${GLM_LIBRARIES}
	${SFML_LIBRARIES}
	${SFML_DEPENDENCIES}
	-llua
	-lpthread
	-lX11
)


if (CMAKE_SYSTEM_NAME MATCHES "Linux")
	add_definitions(-DTARGET_LINUX)
	add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ln -sf  ${CMAKE_SOURCE_DIR}/${RESOURCE_FOLDER} ${CMAKE_BINARY_DIR}/${RESOURCE_FOLDER} )
	add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ln -sf  ${CMAKE_SOURCE_DIR}/*.lua ${CMAKE_BINARY_DIR}/ )
else ()
	file (COPY "${CMAKE_SOURCE_DIR}/${RESOURCE_FOLDER}/" DESTINATION "${CMAKE_BINARY_DIR}/${RESOURCE_FOLDER}")
	file (COPY "${CMAKE_SOURCE_DIR}/*.lua" DESTINATION "${CMAKE_BINARY_DIR}/")
endif ()




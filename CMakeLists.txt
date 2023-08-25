cmake_minimum_required(VERSION 3.26)
project("cppgl")

set(CMAKE_C_STANDARD "11")
set(CMAKE_CXX_STANDARD "20")

file(GLOB_RECURSE SRC_FILES 
	"src/*.c"
	"src/*.h"
	"src/*.cpp"
	"src/*.hpp"
	"src/*.cxx"
	"src/*.hxx"
)

add_executable("${PROJECT_NAME}" "${SRC_FILES}")

find_package(OpenGL REQUIRED)
target_link_libraries("${PROJECT_NAME}" OpenGL::GL)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("glfw")
target_link_libraries("${PROJECT_NAME}" "glfw" "${GLFW_LIBRARIES}")

add_subdirectory("glm")
target_link_libraries("${PROJECT_NAME}" "glm::glm" "${GLM_LIBRARIES}")
if(WIN32) 
add_definitions("-D_CRT_SECURE_NO_WARNINGS")
endif()

add_subdirectory("glad")
target_link_libraries("${PROJECT_NAME}" "glad")

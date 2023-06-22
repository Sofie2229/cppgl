cmake_minimum_required(VERSION 3.26)

project("main")

set(CMAKE_CXX_STANDARD "20")

file(GLOB_RECURSE SRC_FILES 
	"*.c"
	"*.h"
	"*.cpp"
	"*.hpp"
	"*.cxx"
	"*.hxx"
)


add_executable("${PROJECT_NAME}" "${SRC_FILES}")

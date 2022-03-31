cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

add_compile_options(-o2)

project(Rail)

include_directories(include)

add_library(LineaFerroviaria
	include/LineaFerroviaria.h
	src/LineaFerroviaria.cpp)

add_library(Stazione
	include/Stazione.h
	src/Stazione.cpp)

add_library(Treno
	include/Treno.h
	src/Treno.cpp)

add_executable(Rail
	src/main.cpp)

target_link_libraries(Rail
	LineaFerroviaria
	Stazione
	Treno)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/line_description.txt
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/timetables.txt
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
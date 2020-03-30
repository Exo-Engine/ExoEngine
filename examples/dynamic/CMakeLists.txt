cmake_minimum_required(VERSION 3.8)
project(ExoEngine CXX)

file(GLOB SOURCES
	*.h
	*.cpp
)

link_libraries(ExoEngine)

add_executable(dynamic ${SOURCES})

cmake_minimum_required(VERSION 3.8)
project(ExoEngine CXX)

file(GLOB SOURCES
	*.h
	*.cpp
)

link_libraries(ExoEngine)

file(GLOB RESOURCES
	resources/*
)

file(COPY ${RESOURCES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources)

add_executable(window ${SOURCES})

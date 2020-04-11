cmake_minimum_required(VERSION 3.8)
project(ExoEngine CXX)

subdirs(examples)

add_subdirectory(third_party/bullet3)
add_subdirectory(lib-Exo-Renderer)
add_subdirectory(lib-Exo-Audio)

file(GLOB SOURCES
	include/*.h
	include/**/**.h
	src/*.cpp
	src/**/**.cpp
)

include_directories(
	include
	third_party/bullet3/src
	lib-Exo-Renderer/renderer/include
	lib-Exo-Audio/audio/include)

	SET(CMAKE_CXX_FLAGS  "-Wall -Wextra")

link_libraries(SDL2_net ssl crypto dl pthread xml2 BulletDynamics BulletCollision LinearMath)

add_library(ExoEngine STATIC ${SOURCES})

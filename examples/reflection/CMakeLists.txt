cmake_minimum_required(VERSION 3.8)
project(ExoEngine CXX)

file(GLOB SOURCES
	*.h
	*.cpp
)

link_libraries(ExoEngine SDL2 SDL2_net ssl crypto xml2 BulletDynamics BulletCollision LinearMath)

# include_directories(
# 	include
# 	third_party/bullet3/src
# 	lib-Exo-Renderer/renderer/include
# 	lib-Exo-Audio/audio/include)

add_executable(reflection ${SOURCES})

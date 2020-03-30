cmake_minimum_required(VERSION 3.8)
project(ExoEngine CXX)

file(GLOB SOURCES
	*.h
	*.cpp
)

link_libraries(ExoEngine SDL2 SDL2_net ssl crypto xml2 BulletDynamics BulletCollision LinearMath)

add_executable(window ${SOURCES})

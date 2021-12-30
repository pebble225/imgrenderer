cmake_minimum_required(VERSION 3.16.0)

project(imgrenderer VERSION 0.1.0)

if(CMAKE_BUILD_TYPE MATCHES Debug)
	add_executable(${PROJECT_NAME} ./src/first.c
		./src/imgrenderer.c
	)
else()
	add_library(${PROJECT_NAME} ./src/imgrenderer.c)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC include/)

target_link_libraries(${PROJECT_NAME} m)

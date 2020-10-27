cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME "SDL2-Project")

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)

if(NOT Apple)
	find_package(OpenGL)
endif()

add_subdirectory(SDL2)

include_directories("${CMAKE_SOURCE_DIR}/SDL2/include")

file(GLOB PROJECT_SOURCES *.h *.cpp)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} SDL2 OpenGL::GL)

set (source "${CMAKE_SOURCE_DIR}/Assets")
set (destination "${CMAKE_CURRENT_BINARY_DIR}/Assets")
add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination}
	DEPENDS ${destination}
	COMMENT "symbolic link assets folder from ${source} to ${destination}"
)


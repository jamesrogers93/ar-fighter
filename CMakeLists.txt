cmake_minimum_required(VERSION 2.8.4)

#project(ar-fighter)

option(STATIC "Build as a static library" ON)

if(IOS)
	message(STATUS "Compiling for iOS")
elseif(ANDROID)
	message(STATUS "Compiling for Android")
else()
	message(STATUS "Compiling for this system")
endif()

# Output library location
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# GameEngine dependency static library
add_subdirectory(game-engine)

# cmake modules for ARFighter
#set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/game-engine/cmake/modules")

# ARFighter dynamic library
add_subdirectory(ar-fighter)
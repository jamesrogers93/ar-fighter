cmake_minimum_required(VERSION 2.8.4)

#project(ar-fighter)

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


# Game engine dependency
add_subdirectory(game-engine)

add_subdirectory(ar-fighter)
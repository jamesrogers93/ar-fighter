
project(ar-fighter)

# Finds all header and source files and places in AR_FIGHTER_INC and AR_FIGHTER_SRC respectively
include(include/CMakeLists.txt)
include(src/CMakeLists.txt)

# include ar-fighter directories
include_directories(${CMAKE_SOURCE_DIR}/ar-fighter/include)

# Include game-engine include
include_directories(${CMAKE_SOURCE_DIR}/game-engine/game-engine/include) 
#list(APPEND AR-FIGHTER_DEBUG_LIBRARIES
#    ${CMAKE_SOURCE_DIR}/build.macOS/lib/Debug/libGameEngine.a
#)
#list(APPEND AR-FIGHTER_RELEASE_LIBRARIES
#    ${CMAKE_CURRENT_BINARY_DIR}/../lib/Release/libGameEngine.a
#)


# Add platform specific definitions
if(IOS)
	add_definitions(-D__IOS__)
	set(PLATFORM "iOS")
elseif(ANDROID)
	add_definitions(-D__ANDROID__)
	set(PLATFORM "android")
else()
	add_definitions(-D__MACOS__)
	set(PLATFORM "macOS")
endif()

# Platform specific external libraries
if(IOS OR  ANDROID)
	# Include OpenGLES
	find_package(OpenGLES REQUIRED)
	#include_directories("${OPENGLES_INCLUDE_DIR}")
	list(APPEND ARFIGHTER_LIBRARIES
            ${OPENGLES_LIBRARIES}
        )
else()
	# Include OpenGL
	find_package(OpenGL REQUIRED)
	include_directories("${OPENGL_INCLUDE_DIR}")
	list(APPEND ARFIGHTER_LIBRARIES
    	    ${OPENGL_LIBRARY}
        )
endif()

# Include GLM (same for all platforms)
find_package (glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

# Make the ARFighter library
add_library(ARFighter SHARED ${AR_FIGHTER_INC} ${AR_FIGHTER_SRC})

# Macro to take the file directories of the files and add them to groups to maintain the folder structure in an IDE.
macro(build_groups_ARFighter FILES_LIST)
	foreach(FILE ${FILES_LIST}) 
  		get_filename_component(PARENT_DIR "${FILE}" PATH)

  		# skip src or include and changes /'s to \\'s
  		string(REGEX REPLACE "(\\./)?(src|include|ar-fighter)/?" "" GROUP "${PARENT_DIR}")
  		string(REPLACE "/" "\\" GROUP "${GROUP}")

  		# group into "Source Files" and "Header Files"
  		if ("${FILE}" MATCHES ".*\\.cpp")
  	  		set(GROUP "Source Files\\${GROUP}")
  		elseif("${FILE}" MATCHES ".*\\.h")
  	  		set(GROUP "Header Files\\${GROUP}")
  		endif()

  		source_group("${GROUP}" FILES "${FILE}")
	endforeach()
endmacro()

# Group files
build_groups_ARFighter("${AR_FIGHTER_INC}")
build_groups_ARFighter("${AR_FIGHTER_SRC}")

# Install
install(TARGETS ARFighter DESTINATION ../lib/$(PLATFORM))
install(FILES ${AR_FIGHTER_INC} DESTINATION include)

# Link Libraries
target_link_libraries(ARFighter GameEngine)
target_link_libraries(ARFighter $(ARFIGHTER_LIBRARIES))
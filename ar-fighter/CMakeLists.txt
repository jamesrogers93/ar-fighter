
project(ar-fighter)

# include ar-fighter directories
include_directories(${CMAKE_SOURCE_DIR}/ar-fighter/include)

# Finds all header and source files and places in AR_FIGHTER_INC and AR_FIGHTER_SRC respectively
include(include/CMakeLists.txt)
include(src/CMakeLists.txt)

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
#if(IOS OR ANDROID)
	# Include OpenGLES
	#find_package(OpenGLES REQUIRED)
	#include_directories("${OPENGLES_INCLUDE_DIR}")
	#list(APPEND PLATFORM_LIBRARIES
        #    ${OPENGLES_LIBRARIES}
        #)
#else()
	# Include OpenGL
	#find_package(OpenGL REQUIRED)
	#include_directories("${OPENGL_INCLUDE_DIR}")
	#list(APPEND PLATFORM_LIBRARIES
        #    ${OPENGL_LIBRARY}
        #)
#endif()

add_library(ARFighter SHARED ${AR_FIGHTER_INC} ${AR_FIGHTER_SRC})

## Group files

# Macro to take the file directories of the files and add them to groups to maintain the folder structure in an IDE.
macro(build_groups FILES_LIST)
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

# Call the macro above
build_groups("${AR_FIGHTER_INC}")
build_groups("${AR_FIGHTER_SRC}")

install(TARGETS ARFighter DESTINATION ../lib/$(PLATFORM))
install(FILES ${AR_FIGHTER_INC} DESTINATION include)

# Link Libraries
#target_link_libraries(ARFighter ${PLATFORM_LIBRARIES})



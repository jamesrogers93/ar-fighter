
	message(STATUS "Looking for OpenGLES")
if(IOS)
	message(STATUS "Looking for OpenGLES for iOS")

	#find_path( OPENGLES_INCLUDE_DIR
        #	OpenGLES/ES3/gl.h
   	#)
	#set(OPENGLES_INCLUDE_DIR "")

    	find_path( OPENGLES_FRAMEWORKS OpenGLES )

    	if(OPENGLES_FRAMEWORKS)
        	set( OPENGLES_LIBRARIES "-framework OpenGLES" )
    	endif()

elseif(ANDROID)

	#find_path( OPENGLES_INCLUDE_DIR
        #	GLES3/gl3.h
        #	"${ANDROID_STANDALONE_TOOLCHAIN}/usr/include"
    	#)

    	find_library( OPENGLES_LIBRARIES
        	NAMES
            		GLESv3
        	PATHS
            		"${ANDROID_STANDALONE_TOOLCHAIN}/usr/lib"
    	)

endif()

set( OPENGLES_FOUND "NO" )
if(OPENGLES_LIBRARIES)
    set( OPENGLES_FOUND "YES" )
    message(STATUS "Found OpenGLES")
endif(OPENGLES_LIBRARIES)
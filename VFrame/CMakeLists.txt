file(GLOB_RECURSE SOURCES_VGF "*.c*")
message(STATUS "Setting up VFrame Project")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

link_directories(${CMAKE_CURRENT_BINARY_DIR})

find_package(SFML 3 REQUIRED COMPONENTS System Window Graphics Network Audio Main)
if (SFML_FOUND)	
	find_path(SFML_INCLUDE_DIR SFML/Config.hpp
          PATH_SUFFIXES include
          PATHS ${FIND_SFML_PATHS})
	include_directories(${SFML_INCLUDE_DIR})

	if (SFML_STATIC_LIBRARIES)
		add_definitions(-DSFML_STATIC)
	endif()
endif()

find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
	include_directories(${OPENGL_INCLUDE_DIRS})
endif()

find_package(GLEW )
if (GLEW_FOUND)
	include_directories(${GLEW_INCLUDE_DIR})
	if (GLEW_USE_STATIC_LIBS)
		add_definitions(-DGLEW_STATIC)
	endif()
else()
	add_definitions(-DVFRAME_NO_3D)
endif()

find_package(CHIPMUNK2D )
if (CHIPMUNK2D_FOUND)
	include_directories(${CHIPMUNK2D_INCLUDE_DIR})
	link_libraries(${CHIPMUNK2D_LIBRARY})
else()
	add_definitions(-DVFRAME_NO_PHYSICS)
endif()

find_package(CANDLE )
if (CANDLE_FOUND)
	include_directories(${CANDLE_INCLUDE_DIR})
	link_libraries(${CANDLE_LIBRARY})
else()
	add_definitions(-DVFRAME_NO_CANDLE)
endif()

if (UNIX)
	find_package(X11 )
	if (X11_FOUND) 
		include_directories(${X11_INCLUDE_DIRS})
	endif()
	link_libraries(${X11_LIBRARIES})
endif()

if (WIN32)
	link_libraries(winmm.lib)
endif()

include_directories(.)
add_library(vigilante STATIC ${SOURCES_VGF})
target_compile_features(vigilante PUBLIC cxx_std_17)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES $<TARGET_FILE_DIR:vigilante>/Example/Assets)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES $<TARGET_FILE_DIR:vigilante>/VFrame)

if(${WIN32})
	file( WRITE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.vcxproj.user" 
	    "<?xml version=\"1.0\" encoding=\"utf-8\"?>     \
	    <Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">
	    	<PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">
	    	    <LocalDebuggerWorkingDirectory>$(OutDir)</LocalDebuggerWorkingDirectory>
	    	    <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
	    	</PropertyGroup>
	    	<PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">
	    	    <LocalDebuggerWorkingDirectory>$(OutDir)</LocalDebuggerWorkingDirectory>
	    	    <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
	    	</PropertyGroup>
	    	<PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">
	 	    <LocalDebuggerWorkingDirectory>$(SolutionDir)</LocalDebuggerWorkingDirectory>
		    <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
		</PropertyGroup>
		<PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">
		    <LocalDebuggerWorkingDirectory>$(SolutionDir)</LocalDebuggerWorkingDirectory>
		    <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
		</PropertyGroup>
	   </Project>")
endif(${WIN32})

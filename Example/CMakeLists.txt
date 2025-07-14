file(GLOB_RECURSE SOURCES_DEMO "*.cpp")
file(REMOVE_RECURSE "*.NX.cpp")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
message(STATUS "Setting up Example Project")

find_package(SFML 3 REQUIRED COMPONENTS System Window Graphics Network Audio Main REQUIRED)
if (SFML_FOUND)
	find_path(SFML_INCLUDE_DIR SFML/Config.hpp
          PATH_SUFFIXES include
          PATHS ${FIND_SFML_PATHS})

	include_directories(${SFML_INCLUDE_DIR})
	message(STATUS "Linking SFML Libraries...")
	if (SFML_SYSTEM_FOUND)
		link_libraries(SFML::System)
		message(STATUS "SFML::System")
	endif()
	if (SFML_WINDOW_FOUND)
		link_libraries(SFML::Window)
		message(STATUS "SFML::Window")
	endif()
	if (SFML_GRAPHICS_FOUND)
		link_libraries(SFML::Graphics)
		message(STATUS "SFML::Graphics")
	endif()
	if (SFML_NETWORK_FOUND)
		link_libraries(SFML::Network)
		message(STATUS "SFML::Network")
	endif()
	if (SFML_AUDIO_FOUND)
		link_libraries(SFML::Audio)
		message(STATUS "SFML::Audio")
	endif()
	if (SFML_MAIN_FOUND)
		link_libraries(SFML::Main)
		message(STATUS "SFML::Main")
	endif()

	if (SFML_STATIC_LIBRARIES)
		add_definitions(-DSFML_STATIC)
	endif()
endif()

find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
	include_directories(${OPENGL_INCLUDE_DIRS})
	link_libraries(${OPENGL_LIBRARIES})
	message(STATUS "OPENGL library: ${OPENGL_LIBRARIES}.")
endif()

find_package(GLEW)
if (GLEW_FOUND)
	link_libraries(${GLEW_LIBRARIES})
	message(STATUS "GLEW libraries: ${GLEW_LIBRARIES}.")
	if (GLEW_USE_STATIC_LIBS)
		add_definitions(-DGLEW_STATIC)
	endif()
else()	
	message(STATUS "GLEW not found, disabling 3D rendering.")
	add_definitions(-DVFRAME_NO_3D)
endif()

find_package(CHIPMUNK2D )
if (CHIPMUNK2D_FOUND)
	message(STATUS "CHIPMUNK2D library: ${CHIPMUNK2D_LIBRARY}.")
else()
	add_definitions(-DVFRAME_NO_PHYSICS)
endif()

find_package(CANDLE )
if (CANDLE_FOUND)
	message(STATUS "CANDLE library: ${CANDLE_LIBRARY}.")
else()
	add_definitions(-DVFRAME_NO_PHYSICS)
endif()

include_directories(.)
add_executable(vigilantedemo ${SOURCES_DEMO})
target_link_libraries(vigilantedemo vigilante)
target_compile_features(vigilantedemo PUBLIC cxx_std_17)

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

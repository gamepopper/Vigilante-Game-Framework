file(GLOB_RECURSE SOURCES_VGF "*.cpp")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

link_directories(${CMAKE_CURRENT_BINARY_DIR})

find_package(SFML 2.4 REQUIRED system window graphics network audio main)
if (SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIR})
	link_libraries(${SFML_LIBRARIES})
endif()

find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
	include_directories(${OPENGL_INCLUDE_DIRS})
	link_libraries(${OPENGL_LIBRARIES})
endif()

find_package(PNG)
if (PNG_FOUND)
	include_directories(${PNG_INCLUDE_DIRS})
endif()
link_libraries(${PNG_LIBRARIES})

find_package(JPEG)
if (JPEG_FOUND)
	include_directories(${JPEG_INCLUDE_DIRS})
endif()
link_libraries(${JPEG_LIBRARY})

find_package(OpenAL)
if (OPENAL_FOUND)
	include_directories(${OPENAL_INCLUDE_DIRS})
endif()
link_libraries(${OPENAL_LIBRARY})

find_package(OGG )
if (OGG_FOUND)
	include_directories(${OGG_INCLUDE_DIRS})
endif()
link_libraries(${OGG_LIBRARIES})

find_package(VORBIS )
if (VORBIS_FOUND)
	include_directories(${VORBIS_INCLUDE_DIRS})
endif()
link_libraries(${VORBIS_LIBRARIES})

find_package(FLAC )
if (FLAC_FOUND)
	include_directories(${FLAC_INCLUDE_DIRS})
endif()
link_libraries(${FLAC_LIBRARY})

find_package(Freetype )
if (FREETYPE_FOUND)
	include_directories(${FREETYPE_INCLUDE_DIRS})
endif()
link_libraries(${FREETYPE_LIBRARY})

find_package(GLEW)
if (GLEW_FOUND)
	include_directories(${GLEW_INCLUDE_DIR})
	link_libraries(${GLEW_LIBRARY})
else()
	add_definitions(-DVFRAME_NO_3D)
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
	    <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='MinSizeRel|x64'\">
	        <LocalDebuggerWorkingDirectory>$(OutDir)</LocalDebuggerWorkingDirectory>
	        <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
	    </PropertyGroup>
	    <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">
	        <LocalDebuggerWorkingDirectory>$(OutDir)</LocalDebuggerWorkingDirectory>
	        <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
	    </PropertyGroup>
	    <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='RelWithDebInfo|x64'\">
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

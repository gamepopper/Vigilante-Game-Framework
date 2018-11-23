file(GLOB_RECURSE SOURCES_DEMO "*.cpp")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(SFML 2.4.2 REQUIRED system window graphics network audio)
if (SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIR})
	link_libraries(${SFML_LIBRARIES})
endif()

find_package(GLEW)
if (GLEW_FOUND)
	include_directories(${GLEW_INCLUDE_DIR})
	link_libraries(${GLEW_LIBRARY})
else()
	add_definitions(-DVFRAME_NO_3D)
endif()

find_package(CHIPMUNK2D)
if (CHIPMUNK2D_FOUND)
else()
	add_definitions(-DVFRAME_NO_PHYSICS)
endif()

include_directories(.)
add_executable(vigilantedemo ${SOURCES_DEMO})
target_link_libraries(vigilantedemo vigilante)

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
	    </Project>")
endif(${WIN32})

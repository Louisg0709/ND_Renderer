cmake_minimum_required(VERSION 3.30.0)
project(NdRenderer)

add_subdirectory(glfw)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL  "GLFW lib only")
set(GLFW_BUILD_TESTS OFF CACHE BOOL  "GLFW lib only")
set(GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only")
set(GLFW_BUILD_INSTALL OFF CACHE BOOL  "GLFW lib only")

add_subdirectory(DiligentCore)

set(SourceFiles
	src/App.cpp
	include/App.h
	src/GraphicsHandler.cpp
	include/GraphicsHandler.h
	src/MyMaths.cpp
	include/MyMaths.h
)

add_library(NdRenderer STATIC ${SourceFiles})

target_link_libraries(NdRenderer
	glfw
	Diligent-GraphicsEngine
	Diligent-GraphicsEngineVk-static
)

include_directories("glfw/include")
include_directories(DiligentCore)
include_directories(include)

add_executable(Test src/main.cpp)
target_link_libraries(Test NdRenderer)

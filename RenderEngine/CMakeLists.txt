################################################
############# RENDERER DLL PROJECT #############
################################################

cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)

project(RenderEngine)
message("------------------START OF TEST RENDER ENGINE CMAKELIST------------------")


# Headers and pahts
set(CPP_INCLUDE_DIRS "")
set(CPP_SOURCE_FILES "")
set(CPP_HEADER_FILES "")

# Add our project files
file(GLOB_RECURSE CPP_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/../include/imgui/*.cpp")
file(GLOB_RECURSE CPP_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/../include/imgui/*.h")

# Setup include directories
list(APPEND CPP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../include/imgui/")
list(APPEND CPP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../src/")


# Includ directory
include_directories(${CPP_INCLUDE_DIRS}) 

# Executable
add_library(${PROJECT_NAME} SHARED
	${CPP_HEADER_FILES} 
	${CPP_SOURCE_FILES}
)

# Add preprocessor definitions based on configuration/build type
add_compile_definitions("-DBUILD_DLL $<$<CONFIG:Debug>:-D_Debug>")
#add_compile_definitions("-BUILD_DLL")
#add_compile_definitions("$<$<CONFIG:Release>:-Release>")

#build_command()

message("------------------END OF TEST APP CMAKELIST------------------")

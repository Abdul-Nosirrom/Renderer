################################################
############# MAIN PROJECT #####################
################################################

cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)

#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) #link time optimization

project(RendererProject)
message("------------------START OF RENDERER CMAKELIST------------------")
#set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

#set(CMAKE_BUILD_TYPE "Debug")

message("Generated with config types: ${CMAKE_CONFIGURATION_TYPES}")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")

set(CMAKE_C_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

# Externals
#message("Eigen include dir = ${Eigen_INCLUDE_DIRS}")

# Executable
add_subdirectory(RenderEngine)
add_subdirectory(tests)
#add_library(RenderEngine)
#add_executable(TestApplication)

#target_link_libraries(RenderEngine)
 
# Add preprocessor definitions based on configuration/build type
#add_compile_definitions("$<$<CONFIG:Debug>:-_Debug>")
#add_compile_definitions("$<$<CONFIG:Release>:-Release>")

message("------------------END OF RENDERER CMAKELIST------------------")

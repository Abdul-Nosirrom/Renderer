################################################
############# TEST PROJECT #####################
################################################

cmake_minimum_required(VERSION 3.20)


#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) #link time optimization


project(TestApplication)
message("------------------START OF TEST APPLICATION CMAKELIST------------------")
#set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

#set(CMAKE_BUILD_TYPE "Debug")

message("Generated with config types: ${CMAKE_CONFIGURATION_TYPES}")

# Headers and pahts
set(CPP_INCLUDE_DIRS "")
set(CPP_SOURCE_FILES "")
set(CPP_HEADER_FILES "")

# This section deal with the character sets UNICODE _UNICODE
# Directx win32 settings
# this deal with convert types
#ADD_DEFINITIONS(-DUNICODE)
#ADD_DEFINITIONS(-D_UNICODE)
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /UMBCS /D_UNICODE /DUNICODE")

# Directx win32 settings
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")

set(CMAKE_C_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

# Add ImGui directly, not making a lib file from it or anything so we don't have a cmake for it
#file(GLOB_RECURSE CPP_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/imgui/*.cpp")
#file(GLOB_RECURSE CPP_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/imgui/*.h")

# Add our project files
file(GLOB_RECURSE CPP_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/../include/imgui/*.cpp")
file(GLOB_RECURSE CPP_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/../include/imgui/*.h")

# Setup include directories
#list(APPEND CPP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include/imgui/")
list(APPEND CPP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/")
list(APPEND CPP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../RenderEngine/src/")
list(APPEND CPP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../include/imgui/")

# Includ directory
include_directories(${CPP_INCLUDE_DIRS}) 

# Executable
add_executable(${PROJECT_NAME}
	${CPP_HEADER_FILES} 
	${CPP_SOURCE_FILES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE RenderEngine)


# Copy dll to executable [https://stackoverflow.com/questions/10671916/how-to-copy-dll-files-into-the-same-folder-as-the-executable-using-cmake]
#add_custom_command(
#    TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
#    COMMAND_EXPAND_LISTS
#)

# Copy static libs to executable
#add_custom_command(
#    TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_LIBS:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
#    COMMAND_EXPAND_LISTS
#)

# Copy resource directory into executable directory
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources/
)

# Copy shader directory into executable directory
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders/
)

# Post build cleannup command
# Perform build cleanup
#add_custom_command(
#    TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND ${CMAKE_COMMAND} ${PROJECT_BINARY_DIR} --clean-first 
#) 


# Add preprocessor definitions based on configuration/build type
#add_compile_definitions("$<$<CONFIG:Debug>:-_Debug>")
#add_compile_definitions("$<$<CONFIG:Release>:-Release>")

message("------------------END OF TEST APP CMAKELIST------------------")

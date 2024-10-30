################################################
############# TEST PROJECT #####################
################################################

cmake_minimum_required(VERSION 3.20)


set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) #link time optimization


project(TestApplication)
message("------------------START OF TEST APPLICATION CMAKELIST------------------")

message("Generated with config types: ${CMAKE_CONFIGURATION_TYPES}")

# Headers and pahts
set(CPP_INCLUDE_DIRS "")
set(CPP_SOURCE_FILES "")
set(CPP_HEADER_FILES "")

# Directx win32 settings
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")

set(CMAKE_C_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add our project files
file(GLOB_RECURSE CPP_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")# "${CMAKE_CURRENT_SOURCE_DIR}/../RenderEngine/external/imgui/*.cpp")
file(GLOB_RECURSE CPP_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")# "${CMAKE_CURRENT_SOURCE_DIR}/../RenderEngine/external/imgui/*.h")

# Setup include directories
#list(APPEND CPP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include/imgui/")
list(APPEND CPP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/")
list(APPEND CPP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../RenderEngine/src/")
list(APPEND CPP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../external/imgui/")

# Includ directory
include_directories(${CPP_INCLUDE_DIRS}) 

# Executable
add_executable(${PROJECT_NAME}
	${CPP_HEADER_FILES} 
	${CPP_SOURCE_FILES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE GeometryProcessing)
target_link_libraries(${PROJECT_NAME} PRIVATE RenderEngine)
target_link_libraries(${PROJECT_NAME} PRIVATE assimp)

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

message("------------------END OF TEST APP CMAKELIST------------------")

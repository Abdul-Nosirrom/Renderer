################################################
############# RENDERER DLL #####################
################################################

project(RenderEngine)
message("------------------START OF TEST RENDER ENGINE CMAKELIST------------------")

# Headers and paths
set(CPP_INCLUDE_DIRS "")
set(CPP_SOURCE_FILES "")
set(CPP_HEADER_FILES "")

# Add our project files
file(GLOB_RECURSE CPP_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/../external/imgui/*.cpp")
file(GLOB_RECURSE CPP_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/../external/imgui/*.h")

# Setup include directories
list(APPEND CPP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../external/imgui/")
list(APPEND CPP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../external/eigen")
list(APPEND CPP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/")

# Includ directory
include_directories(${CPP_INCLUDE_DIRS}) 

# Executable
add_library(${PROJECT_NAME} SHARED
	${CPP_HEADER_FILES} 
	${CPP_SOURCE_FILES}
)

target_link_libraries(${PROJECT_NAME} INTERFACE eigen)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../external/assimp/include")
target_link_libraries(${PROJECT_NAME} PUBLIC assimp)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../GeometryProcessing/src")
target_link_libraries(${PROJECT_NAME} PUBLIC GeometryProcessing)

# Add preprocessor definitions based on configuration/build type
add_compile_definitions("-DBUILD_DLL $<$<CONFIG:Debug>:-D_Debug>")

message("------------------END OF TEST APP CMAKELIST------------------")

﻿################################################
############# GEOMETRY PROCESSING LIB #####################
################################################

project(GeometryProcessing)
message("------------------START OF GEOMETRY PROCESSING CMAKELIST------------------")

# Headers and pahts
set(CPP_INCLUDE_DIRS "")
set(CPP_SOURCE_FILES "")
set(CPP_HEADER_FILES "")

# Add our project files
file(GLOB_RECURSE CPP_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE CPP_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

# Setup include directories
list(APPEND CPP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../external/eigen")
#list(APPEND CPP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/include")
list(APPEND CPP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/")

# Includ directory
include_directories(${CPP_INCLUDE_DIRS})

# Executable
add_library(${PROJECT_NAME}
        ${CPP_HEADER_FILES}
        ${CPP_SOURCE_FILES}
)

target_link_libraries(${PROJECT_NAME} INTERFACE eigen)

# Add preprocessor definitions based on configuration/build type
add_compile_definitions("-DBUILD_DLL $<$<CONFIG:Debug>:-D_Debug>")
#add_compile_definitions("-BUILD_DLL")
#add_compile_definitions("$<$<CONFIG:Release>:-Release>")

#build_command()

message("------------------END OF TEST APP CMAKELIST------------------")
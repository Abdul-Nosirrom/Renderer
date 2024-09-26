################################################
############# EXTERNALS [.H] PROJECT ###########
################################################

cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)

project(externals)
add_subdirectory(eigen)

# Eigen as interface header only library
#project(Eigen) 
#set(${Eigen_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/eigen/)
#set(${Eigen_SOURCE_DIRS} "")
#file(GLOB_RECURSE EIGEN_SOURCE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/eigen/Eigen/*")
#add_library(${PROJECT_NAME} INTERFACE ${EIGEN_SOURCE_DIRS})
#target_include_directories(${PROJECT_NAME} INTERFACE ${Eigen_INCLUDE_DIRS})
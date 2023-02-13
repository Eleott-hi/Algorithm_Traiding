cmake_minimum_required(VERSION 3.5)
project(common VERSION 0.1 LANGUAGES CXX)

# add_compile_options(-O3)
file(GLOB_RECURSE SOURCE_FILES *.cc *.h)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

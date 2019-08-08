cmake_minimum_required(VERSION 3.14)
project(subsolar)

set(CMAKE_CXX_STANDARD 14)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99") # enable C99 standard
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14") # enable C++11 standard

set( CMAKE_VERBOSE_MAKEFILE on )

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(include)

add_executable(subsolar src/main.cpp include/sokol_gfx.h src/sokol_gfx.m)

target_link_libraries(subsolar ${SDL2_LIBRARIES})
target_link_libraries(subsolar ${OPENGL_LIBRARIES})

#set_property (TARGET subsolar APPEND_STRING PROPERTY COMPILE_FLAGS "-fobjc-arc ")
set_property (TARGET subsolar APPEND_STRING PROPERTY COMPILE_FLAGS "-Wno-address-of-temporary ")

set(CMAKE_XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC "YES")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fobjc-arc")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fobjc-arc")

target_link_libraries(subsolar "-framework Metal" "-framework Cocoa" "-framework MetalKit" "-framework Quartz")


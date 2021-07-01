cmake_minimum_required(VERSION 3.10.2)
project(libvmcu_Virtual_MCU_Library C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS_RELEASE "-O1")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(.)
include_directories(engine/include/libvmcu)

add_definitions(-DATMEGA328P)

##### libvmcu - static archive #####

file(GLOB_RECURSE HEADER engine/include/*.h)
file(GLOB_RECURSE SOURCE engine/src/*.c)

add_library(vmcu ${HEADER} ${SOURCE})

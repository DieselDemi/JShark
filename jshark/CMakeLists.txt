cmake_minimum_required(VERSION 3.21)

project(JShark)

set(CMAKE_CXX_STANDARD 20)

set(SRCS
    JSharkDLL.h
    stdafx.cpp
    DLLMain.cpp
    Registry.cpp
    Helpers.cpp
    Logger.cpp
    JShark.cpp
    JShark.def
)

add_library(JShark SHARED ${SRCS})

install(TARGETS JShark
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
cmake_minimum_required(VERSION 3.26)

add_executable(28_4_3
        ../main.cpp
        ../headers/constants.h
        ../headers/utilities.h
        ../headers/threads.h
        utilities.cpp
        threads.cpp
)

target_include_directories(28_4_3 PUBLIC ${CMAKE_SOURCE_DIR}/headers)
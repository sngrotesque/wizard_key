cmake_minimum_required(VERSION 3.19)
project(AES)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

set(CMAKE_C_FLAGS "-std=c99")

add_executable(AES
        main.c src/mode.c src/AES.c src/utils.c)
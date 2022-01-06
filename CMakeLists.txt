cmake_minimum_required(VERSION 3.20)
project(CConnect4 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS)

include_directories(.)

add_executable(CConnect4
        main.c
        window.h game.h)

target_link_libraries(CConnect4 SDL2main SDL2)



cmake_minimum_required(VERSION 3.20)
project(CConnect4 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-O0 -Wall -Wextra")

include_directories(.)

add_executable(CConnect4
        main.c
        render.c render.h logic.c logic.h)

target_link_libraries(CConnect4 SDL2main SDL2 SDL2_gfx SDL2_ttf SDL2_mixer)

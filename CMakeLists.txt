cmake_minimum_required(VERSION 3.27)
project(Snake C)

set(CMAKE_C_STANDARD 23)

add_executable(Snake main.c
        GameManager.c
        Snake.c
        Apple.c
        Util.c)

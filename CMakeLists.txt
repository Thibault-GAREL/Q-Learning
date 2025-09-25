cmake_minimum_required(VERSION 3.23)
project(Renforcement_Learning C)

set(CMAKE_C_STANDARD 99)

add_executable(Renforcement_Learning main.c annexe.h)

target_link_libraries(Renforcement_Learning -lmingw32 -lallegro-5.0.10-monolith-md-debug)
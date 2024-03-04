cmake_minimum_required(VERSION 3.16)

project(lab1)

add_executable(main main.cpp sorts.cpp time_checkers.cpp sorts.h chekers.h config.h)
target_compile_options(main PRIVATE "-O0")
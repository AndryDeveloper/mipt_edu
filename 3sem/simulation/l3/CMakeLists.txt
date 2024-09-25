cmake_minimum_required(VERSION 3.16)

project(lab3)

#if (WIN32)
if (MSVC)
    # https://docs.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-by-category
    list(APPEND compiler_flags
        "/WX" "/W4" "/Wall" "/permissive-" "/Za" "/fsanitize=address"

        # (not necessary) for runtime library static linkage
        "/MT"

        # (not necessary) use c++20
        "/std=c++2a"
    )
else()
    list(APPEND compiler_flags
        # It is impossible to include all GCC warnings
        # https://stackoverflow.com/a/11720263
        "-Werror" "-Wall" "-Wextra" "-pedantic" "-fsanitize=address"
        "-static-libasan"

        # (not necessary) use c++20
        "-std=c++2a"
    )
    list(APPEND linker_flags
        "-fsanitize=address"
        )
endif()

add_executable(task1 task1.cpp)
add_executable(task2 task2.cpp)
add_executable(task3 task3.cpp)
add_executable(task4 task4.cpp)
add_executable(task5 task5.cpp)

target_compile_options(task1 PRIVATE ${compiler_flags})
target_link_options(task1 PRIVATE ${linker_flags})

target_compile_options(task2 PRIVATE ${compiler_flags})
target_link_options(task2 PRIVATE ${linker_flags})

target_compile_options(task3 PRIVATE ${compiler_flags})
target_link_options(task3 PRIVATE ${linker_flags})

target_compile_options(task4 PRIVATE ${compiler_flags})
target_link_options(task4 PRIVATE ${linker_flags})

target_compile_options(task5 PRIVATE ${compiler_flags})
target_link_options(task5 PRIVATE ${linker_flags})


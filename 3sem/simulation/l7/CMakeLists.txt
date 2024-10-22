cmake_minimum_required(VERSION 3.16)

project(s1)

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
        "-Ofast"
        "-Werror" "-Wall" "-Wextra" "-pedantic" "-fsanitize=address"
        "-static-libasan"

        # (not necessary) use c++20
        "-std=c++2a"
    )
    list(APPEND linker_flags
        "-fsanitize=address"
        )
endif()

add_subdirectory(external/json)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)
target_compile_options(main PRIVATE ${compiler_flags})
target_link_options(main PRIVATE ${linker_flags})


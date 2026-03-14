cmake_minimum_required(VERSION 3.16)
project(project)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

# python
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# numpy include directory
execute_process(
        COMMAND ${Python3_EXECUTABLE} -c "import numpy; print(numpy.get_include())"
        OUTPUT_VARIABLE NumPy_INCLUDE_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

message(STATUS "NumPy include dir: ${NumPy_INCLUDE_DIR}")
include_directories(${NumPy_INCLUDE_DIR})

add_library(project_lib
        src/DataFrame.cpp
        src/CSVLoader.cpp
        src/DF_Downloader.cpp
        src/IMG_Downloader.cpp
        src/command.cpp
)

target_include_directories(project_lib
        PUBLIC
            include
            ${Python3_INCLUDE_DIRS}
            ${NumPy_INCLUDE_DIR}
)
target_link_libraries(project_lib PRIVATE ${Python3_LIBRARIES})

# main executable
add_executable(project main.cpp)
target_link_libraries(project PRIVATE project_lib)
target_include_directories(project PRIVATE ${Python3_INCLUDE_DIRS})

option(ENABLE_TESTS "Build Google Tests" OFF)

if(ENABLE_TESTS)
    include(FetchContent)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/refs/heads/main.zip
    )
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    add_executable(core_tests
            tests/test_dataframe.cpp
            tests/test_commands.cpp
    )

    target_link_libraries(core_tests
            PRIVATE
            gtest
            gtest_main
            project_lib
    )

    add_test(NAME core_tests COMMAND core_tests)
endif()

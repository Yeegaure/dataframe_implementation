cmake_minimum_required(VERSION 3.16)
project(project)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

# python
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Find NumPy include directory
execute_process(
        COMMAND ${Python3_EXECUTABLE} -c "import numpy; print(numpy.get_include())"
        OUTPUT_VARIABLE NumPy_INCLUDE_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

message(STATUS "NumPy include dir: ${NumPy_INCLUDE_DIR}")

include_directories(${NumPy_INCLUDE_DIR})

# source files
add_executable(project
        main.cpp
        src/DataFrame.cpp
        src/CSVLoader.cpp
        src/DF_Downloader.cpp
        src/IMG_Downloader.cpp
)

# link python
target_include_directories(project PRIVATE ${Python3_INCLUDE_DIRS})
target_link_libraries(project PRIVATE ${Python3_LIBRARIES})
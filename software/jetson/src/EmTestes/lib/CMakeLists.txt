cmake_minimum_required(VERSION 3.10)
project(compilated_program)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

file(GLOB SOURCES "*.cpp")

add_executable(compilated_program ${SOURCES})

target_include_directories(compilated_program PRIVATE
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(compilated_program
    ${OpenCV_LIBS}
)
target_include_directories(compilated_program PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}  # garante que headers da pasta atual são achados
)

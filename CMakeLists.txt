cmake_minimum_required(VERSION 3.10)
project(SoftwareTinyRender LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)


find_package(OpenMP COMPONENTS CXX)

set(SOURCES src/main.cpp src/tgaimage.cpp src/tgaimage.h )

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE $<$<BOOL:${OpenMP_CXX_FOUND}>:OpenMP::OpenMP_CXX>)

file(GENERATE OUTPUT .gitignore CONTENT "*")

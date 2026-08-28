cmake_minimum_required(VERSION 3.10)
project(SoftwareTinyRender LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


find_package(OpenMP COMPONENTS CXX)

find_package(GLM REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
src/*.cpp
src/*.h)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
glm::glm
)

file(GENERATE OUTPUT .gitignore CONTENT "*")

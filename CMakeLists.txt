cmake_minimum_required(VERSION 3.10)
project(SoftwareTinyRender LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


find_package(OpenMP COMPONENTS CXX)

find_package(GLM REQUIRED)

find_package(SDL3 REQUIRED CONFIG)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
src/*.cpp
src/*.h)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
glm::glm
SDL3::SDL3
)

# Bakes the repo root into the binary so asset paths (obj/...) resolve
# correctly no matter what directory the executable is launched from.
target_compile_definitions(${PROJECT_NAME} PRIVATE
PROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}")

file(GENERATE OUTPUT .gitignore CONTENT "*")

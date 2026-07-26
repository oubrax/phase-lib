cmake_minimum_required(VERSION 3.20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(phase LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  add_compile_options(-Wall -Wextra -O0 -g)
endif()

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glad REQUIRED)

add_library(phase_lib STATIC
  src/widget.cpp
  src/widgets/container/container.cpp
  src/widgets/container/prop.cpp
)

target_include_directories(phase_lib PUBLIC include)
target_link_libraries(phase_lib PUBLIC OpenGL::GL glfw glad::glad)

add_executable(phase src/main.cpp)
target_link_libraries(phase PRIVATE phase_lib)

option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
  enable_testing()
  include(FetchContent)
  FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v2.5.3
  )
  FetchContent_MakeAvailable(doctest)
  add_subdirectory(test)
endif()

cmake_minimum_required(VERSION 3.30)
project(planet)

include(FetchContent)
set(BUILD_SHARED_LIBS OFF)
fetchcontent_declare(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git)
fetchcontent_makeavailable(SFML)

set(CMAKE_CXX_STANDARD 20)

add_executable(planet main.cpp perlin.cpp world.cpp chunk.cpp)

target_link_libraries(planet sfml-graphics)

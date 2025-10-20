cmake_minimum_required(VERSION 3.10)
project(AntColony)

set(CMAKE_CXX_STANDARD 17)

# Include SFML
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# Includes e fontes
include_directories(include)
file(GLOB SOURCES src/*.cpp)

add_executable(AntColony ${SOURCES})
target_link_libraries(AntColony sfml-graphics sfml-window sfml-system)
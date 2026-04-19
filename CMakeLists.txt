cmake_minimum_required(VERSION 3.10)
project(AntColony)

set(CMAKE_CXX_STANDARD 17)

# Include SFML
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# --- ADICIONA ISTO AQUI (Procura o OpenMP no teu PC) ---
find_package(OpenMP REQUIRED)
# -------------------------------------------------------

# Includes e fontes
include_directories(include)
file(GLOB SOURCES src/*.cpp)

add_executable(AntColony ${SOURCES})

# --- ADICIONA O "OpenMP::OpenMP_CXX" NO FIM DESTA LINHA ---
target_link_libraries(AntColony sfml-graphics sfml-window sfml-system OpenMP::OpenMP_CXX)
# ----------------------------------------------------------
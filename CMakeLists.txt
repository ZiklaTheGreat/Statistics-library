cmake_minimum_required(VERSION 3.10)
project(SimulationOutputLibrary VERSION 1.0 LANGUAGES CXX)

# Explicitne nastav politiku pre GLVND (rieši varovanie)
cmake_policy(SET CMP0072 NEW)
set(OpenGL_GL_PREFERENCE "GLVND")  # Preferuj moderné GLVND knižnice

# Nastavenie štandardu C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Konfigurácia Armadilla
set(ARMADILLO_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/third_party/armadillo-code/include")
include_directories(${ARMADILLO_INCLUDE_DIR})

find_library(ARMADILLO_LIB 
    NAMES armadillo 
    HINTS "${CMAKE_SOURCE_DIR}/third_party/armadillo-code"
    NO_DEFAULT_PATH
)

if(NOT ARMADILLO_LIB)
    message(FATAL_ERROR "Armadillo library not found! Build it first in third_party/armadillo-code")
endif()

message(STATUS "Armadillo:")
message(STATUS "  - Includes: ${ARMADILLO_INCLUDE_DIR}")
message(STATUS "  - Library: ${ARMADILLO_LIB}")

# Hlavná knižnica projektu
add_library(simulation_lib INTERFACE)
target_include_directories(simulation_lib INTERFACE 
    ${CMAKE_SOURCE_DIR}/lib/include
)
target_link_libraries(simulation_lib INTERFACE ${ARMADILLO_LIB} blas lapack)

# Konfigurácia Dear ImGui
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/third_party/imgui")

# Statická knižnica ImGui
add_library(imgui STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
)

# Backend pre OpenGL + GLFW
find_package(OpenGL REQUIRED)  # Teraz použije GLVND bez varovaní
find_package(glfw3 REQUIRED)

add_library(imgui_glfw STATIC
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui_glfw PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)
target_link_libraries(imgui_glfw PUBLIC imgui glfw OpenGL::OpenGL)  # Použi OpenGL::OpenGL namiesto OpenGL::GL

# Hlavný program
add_executable(main_app app/main.cpp)
target_link_libraries(main_app PRIVATE simulation_lib imgui_glfw)
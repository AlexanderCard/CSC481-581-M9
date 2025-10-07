cmake_minimum_required(VERSION 3.16)
project(PropertyCentricModelSDL3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)

add_executable(property_model
    src/main.cpp
    src/PropertyTypes.hpp
)

target_link_libraries(property_model PRIVATE SDL3::SDL3)

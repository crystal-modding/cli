cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(crystal VERSION 0.1.0)

add_executable(${PROJECT_NAME} src/main.cpp)

add_subdirectory(nfd-extended)

target_link_libraries(${PROJECT_NAME} PRIVATE
    nfd
)
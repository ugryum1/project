cmake_minimum_required(VERSION 3.10)
project(project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_subdirectory(employeeLib)

add_executable(myapp main.cpp employee_table.cpp)

target_link_libraries(myapp Qt5::Widgets employeeLib)

target_include_directories(myapp PRIVATE employeeLib)

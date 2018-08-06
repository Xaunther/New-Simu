#Version
cmake_minimum_required(VERSION 3.5.1)

#Project name
project(New-simu)

#Programa generado en carpeta bin
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)

#Bring the headers, such as Student.h into the project
include_directories(include)

#Add source files, (.cpp). The file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")

add_executable(New-simu ${SOURCES})

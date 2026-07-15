cmake_minimum_required(VERSION 4.2)
project(untitled C)

set(CMAKE_C_STANDARD 17)

add_executable(untitled main.c
        matrixstruktur.h
        matrixKonstruktion.c
        gausssolver.c
        gaussvalidation.c
        unitTest.c)

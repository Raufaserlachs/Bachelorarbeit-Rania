cmake_minimum_required(VERSION 4.2)
project(untitled C)

set(CMAKE_C_STANDARD 17)

add_executable(untitled main.c
        matrixstruktur.h
        matrixKonstruktion.c
        gausssolver.c
        gaussvalidation.c
        unitTest.c
        gaussValidationCSR.c
        matrixKonventierer.c
        matrixDrucker.c
        speicherFreigabe.c
        guasssolverCSR.c
        vergleiche_csr_und_dichte_loesung.c
        laufzeitmessung.c
        aufwand_messung.c
)

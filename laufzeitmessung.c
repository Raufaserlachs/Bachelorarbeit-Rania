//
// Created by Nia on 12.08.26.
//
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "matrixstruktur.h"


// Diese Funktion misst die Zeit für Konvertierung und Solver getrennt
void messe_performance(FlexibleSparseMatrix sparse) {
    clock_t start, end;
    double zeit_konvert, zeit_solver;

    //  Messung der Konvertierung (Symbolische + Numerische Phase)
    start = clock();
    CSRMatrix csr = konvertiere_zu_optimierten_csr(sparse);
    end = clock();
    zeit_konvert = ((double)(end - start)) / CLOCKS_PER_SEC;




    // Vorbereitung für den Solver
    double *b = calloc(csr.N, sizeof(double));
    double *x = calloc(csr.N, sizeof(double));


    // Einen temporären Test-Vektor x mit 1.0 füllen
    double *x_test = malloc(csr.N * sizeof(double));
    for (int i = 0; i < csr.N; i++) {
        x_test[i] = 1.0;
    }

    // 3. b berechnen: b = A * x_test [1, 2]
    // Wir nutzen hier deine MMVP-Funktion für das CSR-Format [4]
    mmvp_csr(csr, x_test, b);


    // Messung des Gauß-Solvers (ZSF + Rücksubstitution)
    start = clock();
    bringe_in_zeilenstufenform_csr(csr, b);
    loese_rueckwaertssubstitution_csr(csr, b, x);
    end = clock();
    zeit_solver = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Ergebnisse
    printf("\n--- ZEITMESSUNG ---\n");
    printf("Matrix-Groesse (N*B): %d\n", csr.N);
    printf("Zeit Konvertierung:    %f s\n", zeit_konvert);
    printf("Zeit Gauss-Solver:     %f s\n", zeit_solver);
    printf("-------------------\n");

    free(b); free(x);
}
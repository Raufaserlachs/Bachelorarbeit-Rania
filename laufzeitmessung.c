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
    double zeit_konvert, zeit_zsf, zeit_ruecksub;

    // 1. Messung der Konvertierung (Symbolische + Numerische Phase)
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

    // b berechnen: b = A * x_test (Umkehr-Trick)
    mmvp_csr(csr, x_test, b);


    // 2. Messung der Zeilenstufenform (Vorwärts-Elimination)
    start = clock();
    bringe_in_zeilenstufenform_csr(csr, b);
    end = clock();
    zeit_zsf = ((double)(end - start)) / CLOCKS_PER_SEC;


    // 3. Messung der Rücksubstitution
    start = clock();
    loese_rueckwaertssubstitution_csr(csr, b, x);
    end = clock();
    zeit_ruecksub = ((double)(end - start)) / CLOCKS_PER_SEC;


    // Ergebnisse getrennt ausgeben
    printf("\n--- ZEITMESSUNG ---\n");
    printf("Dimension x Blockgrösse: %d\n x %d\n x %d\n x %d\n x %d\n  ", sparse.d1, sparse.d2, sparse.d3, sparse.d4, sparse.B );
    printf(" Matrix-Groesse (N*B): %d\n ", csr.N);

    printf("Zeit symbolische phase:      %f s\n", zeit_konvert);
    printf("Zeit Zeilenstufenform:   %f s\n", zeit_zsf);
    printf("Zeit Ruecksubstitution:  %f s\n", zeit_ruecksub);
    printf("Gesamtzeit ohne symb ph.:       %f s\n", zeit_zsf + zeit_ruecksub);
    printf("Gesamtzeit Gauss Solver insgesamt mit symbolische phase:       %f s\n", zeit_zsf + zeit_ruecksub + zeit_konvert);
    printf("-------------------\n");

    // Speicher aufräumen
    free(x_test);
    free(b);
    free(x);
    freigabe_csr_matrix(csr);
}
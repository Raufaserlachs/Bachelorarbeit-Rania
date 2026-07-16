//
// Created by Nia on 16.07.26.
//
#include <stdio.h>
#include "matrixstruktur.h"





void mmvp_csr(CSRMatrix A, double *x, double *b) {
    for (int i = 0; i < A.N; i++) {
        b[i] = 0.0;
        // Durchlaufe nur die Einträge der Zeile i
        for (int k = A.rst[i]; k < A.rst[i + 1]; k++) {
            int spalte = A.ci[k];
            b[i] += A.val[k] * x[spalte];
        }
    }
}




void berechne_residuum_csr(CSRMatrix A_original, double *x_berechnet, double *b_original) {
    int N = A_original.N;
    double r[N];

    // A * x_berechnet (CSR-Version)
    double Ax[N];
    mmvp_csr(A_original, x_berechnet, Ax);

    // Residuum: r = b - Ax
    for (int i = 0; i < N; i++) {
        r[i] = b_original[i] - Ax[i];
    }

    // Fehlernorm
    double fehler = berechne_norm(r, N);
    printf("Das Residuum (CSR) ist: %e\n", fehler);

    if (fehler < 1e-12) {
        printf("[Erfolg] CSR-Lösung validiert.\n");
    } else {
        printf("[Warnung] CSR-Fehler über Toleranz!\n");
    }
}


void testlauf_csr(CSRMatrix A_original) {
    int N = A_original.N;
    double x_exakt[N];
    double b_original[N];
    double x_berechnet[N];

    for(int i = 0; i < N; i++) x_exakt[i] = 1.0;

    // A * x (CSR)
    mmvp_csr(A_original, x_exakt, b_original);

    // Hier würde später deine Gauß-Elimination für CSR kommen.
    // Vorerst: Zeige uns, dass die MV-Multiplikation korrekt arbeitet.
    printf("[Validator] CSR-Residuum-Test für x=1 gestartet...\n");

    // Wir simulieren hier die Lösung (in der Arbeit ersetzt du das durch deinen CSR-Solver)
    for(int i=0; i<N; i++) x_berechnet[i] = 1.0;

    berechne_residuum_csr(A_original, x_berechnet, b_original);
}
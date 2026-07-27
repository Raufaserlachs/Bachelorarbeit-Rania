//
// Created by Nia on 16.07.26.
//
#include <stdio.h>
#include <stdlib.h>

#include "matrixstruktur.h"




// Hilfsfunktion: Berechnet b = A * x (MVP)
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




// Hilfsfunktion: Kopiert eine CSRMatrix, damit das Original erhalten bleibt
CSRMatrix kopiere_csr_matrix(CSRMatrix A) {
    CSRMatrix kopie;
    kopie.N = A.N;
    kopie.nnz = A.nnz;

    kopie.rst = malloc((A.N + 1) * sizeof(int));
    kopie.ci = malloc(A.nnz * sizeof(int));
    kopie.val = malloc(A.nnz * sizeof(double));

    for (int i = 0; i <= A.N; i++) {
        kopie.rst[i] = A.rst[i];
    }
    for (int k = 0; k < A.nnz; k++) {
        kopie.ci[k] = A.ci[k];
        kopie.val[k] = A.val[k];
    }
    return kopie;
}

// Hilfsfunktion zum Freigeben der CSR-Arbeitskopie
void freigabe_csr_matrix(CSRMatrix A) {
    free(A.rst);
    free(A.ci);
    free(A.val);
}


// Residuum für CSR
void berechne_residuum_csr(CSRMatrix A_original, double *x_berechnet, double *b_original) {
    int N = A_original.N;
    double r[N];

    double Ax[N];
    mmvp_csr(A_original, x_berechnet, Ax);

    for (int i = 0; i < N; i++) {
        r[i] = b_original[i] - Ax[i];
    }

    double fehler = berechne_norm(r, N);
    double schwelle = 1e-12;

    printf("Das Residuum (CSR) ist: %e\n", fehler);

    if (fehler < schwelle) {
        printf("[Erfolg] Die CSR-Lösung ist numerisch stabil und validiert.\n");
    } else {
        printf("[Warnung] Der CSR-Fehler ist über der Toleranzgrenze!\n");
    }
}

// Exakter Nachbau des dichten testlaufs für das CSR-Format
void testlauf_csr(CSRMatrix A_original) {
    int N = A_original.N;

    double x_exakt[N];
    double b_original[N];
    double x_berechnet[N];

    for (int i = 0; i < N; i++) {
        x_exakt[i] = 1.0;
    }

    // b_original erzeugen via CSR-Matrix-Vektor-Multiplikation
    mmvp_csr(A_original, x_exakt, b_original);

    // Arbeitskopie der CSR-Matrix erzeugen, damit das Original intakt bleibt
    CSRMatrix A_work = kopiere_csr_matrix(A_original);
    double b_work[N];
    for (int i = 0; i < N; i++) {
        b_work[i] = b_original[i];
    }

    // Zeilenstufenform auf der Arbeitskopie ausführen
    bringe_in_zeilenstufenform_csr(A_work, b_work);

    // Zustand nach ZSF ausgeben
    printf("\nZustand nach der Zeilenstufenform (CSR ZSF):");
    drucke_csr_matrix(A_work);

    // Rückwärtssubstitution aufrufen
    loese_rueckwaertssubstitution_csr(A_work, b_work, x_berechnet);

    // Berechneten Vektor x ausgeben
    printf("\nBerechneter Vektor x (CSR, Sollte ca. 1.0 sein):\n");
    for (int i = 0; i < N; i++) {
        printf("x[%d] = %20.18f\n", i, x_berechnet[i]);
    }

    // Residuum testen
    printf("[Validator] Starte CSR-Residuum-Test...\n");
    berechne_residuum_csr(A_original, x_berechnet, b_original);

    // Aufräumen
    freigabe_csr_matrix(A_work);
}
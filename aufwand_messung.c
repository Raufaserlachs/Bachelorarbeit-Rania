//
// Created by Nia on 31.08.26.
//

#include <stdio.h>
#include <stdlib.h>
#include "matrixstruktur.h"

void messe_aufwand(FlexibleSparseMatrix sparse) {
    // 1. Konvertierung in CSR
    CSRMatrix csr = konvertiere_zu_optimierten_csr(sparse);

    // 2. Vorbereitung b-Vektor (Umkehr-Trick)
    double *b = calloc(csr.N, sizeof(double));
    double *x = calloc(csr.N, sizeof(double));
    double *x_test = malloc(csr.N * sizeof(double));
    for (int i = 0; i < csr.N; i++) x_test[i] = 1.0;

    mmvp_csr(csr, x_test, b);

    // 3. Zähler vor den Aufrufen auf 0 zurücksetzen!
    op_count_zsf = 0;
    op_count_ruecksub = 0;

    // 4. Solver ausführen (zählt automatisch mit)
    bringe_in_zeilenstufenform_csr(csr, b);
    loese_rueckwaertssubstitution_csr(csr, b, x);

    // 5. Ergebnisse ausgeben
    long long gesamt_aufwand = op_count_zsf + op_count_ruecksub;

    printf("\n========================================\n");
    printf(" AUFWANDSANALYSE (FLOPs / Operationen)\n");
    printf("========================================\n");
    printf("Matrix-Groesse (N*B):       %d\n", csr.N);
    printf("Anzahl Nicht-Nullen (NNZ):  %d\n", csr.nnz);
    printf("----------------------------------------\n");
    printf("FLOPs Zeilenstufenform:     %lld\n", op_count_zsf);
    printf("FLOPs Ruecksubstitution:    %lld\n", op_count_ruecksub);
    printf("Gesamter Rechenaufwand:     %lld FLOPs\n", gesamt_aufwand);
    printf("========================================\n\n");

    // Speicher aufräumen
    free(x_test);
    free(b);
    free(x);
    freigabe_csr_matrix(csr);
}
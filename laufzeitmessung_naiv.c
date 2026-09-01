//
// Created by Nia on 01.09.26.
//
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "matrixstruktur.h"


void messe_aufwand_naiv(FlexibleSparseMatrix sparse) {
    // 1. Konvertierung der sparse Matrix in eine Dichte Matrix
    DichteMatrix dichte = konvertiere_zu_dicht(sparse);

    // 2. Vorbereitung b-Vektor (Umkehr-Trick)
    double *b = calloc(dichte.N, sizeof(double));
    double *x = calloc(dichte.N, sizeof(double));
    double *x_test = malloc(dichte.N * sizeof(double));
    for (int i = 0; i < dichte.N; i++) {
        x_test[i] = 1.0;
    }

    // Rechte Seite erzeugen: b = A * x_test
    mmvp_dichte(dichte, x_test, b);

    // Arbeitskopie der dichten Matrix und von b anlegen
    DichteMatrix dichte_work = kopiere_matrix(dichte);
    double *b_work = malloc(dichte.N * sizeof(double));
    for (int i = 0; i < dichte.N; i++) {
        b_work[i] = b[i];
    }

    // 3. Zähler vor den Aufrufen auf 0 zurücksetzen!
    op_count_naiv_zsf = 0;
    op_count_naiv_ruecksub = 0;

    // 4. Zeilenstufenform (ZSF) ausführen und messen
    clock_t start_zsf = clock();
    bringe_in_zeilenstufenform(dichte_work, b_work);
    clock_t end_zsf = clock();
    double t_zsf = (double)(end_zsf - start_zsf) / CLOCKS_PER_SEC;

    // 5. Rückwärtssubstitution ausführen und messen
    clock_t start_rueck = clock();
    loese_mit_ruecksubstitution(dichte_work, b_work, x);
    clock_t end_rueck = clock();
    double t_rueck = (double)(end_rueck - start_rueck) / CLOCKS_PER_SEC;

    double t_gesamt = t_zsf + t_rueck;
    long long gesamt_aufwand = op_count_naiv_zsf + op_count_naiv_ruecksub;

    // 6. Ergebnisse analog zur CSR-Analyse ausgeben
    printf("\n========================================\n");
    printf(" AUFWANDSANALYSE NAIVER GAUSS-SOLVER\n");
    printf("========================================\n");
    printf("Matrix-Groesse (N):         %d\n", dichte.N);
    printf("----------------------------------------\n");
    printf("Laufzeit Zeilenstufenform:  %.6f s\n", t_zsf);
    printf("Laufzeit Ruecksubstitution: %.6f s\n", t_rueck);
    printf("Gesamte Solver-Laufzeit:    %.6f s\n", t_gesamt);
    printf("----------------------------------------\n");
    printf("FLOPs Zeilenstufenform:     %lld\n", op_count_naiv_zsf);
    printf("FLOPs Ruecksubstitution:    %lld\n", op_count_naiv_ruecksub);
    printf("Gesamter Rechenaufwand:     %lld FLOPs\n", gesamt_aufwand);
    printf("========================================\n\n");

    // 7. Speicher aufräumen
    free(x_test);
    free(b);
    free(b_work);
    free(x);
    freigabe_dichte_matrix(dichte_work);
    freigabe_dichte_matrix(dichte);
}
//
// Created by Nia on 15.07.26.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Matrixstruktur.h"

// Hilfsfunktion: Erstellt eine Matrix mit Werten, die wir exakt kennen
FlexibleSparseMatrix erzeuge_einfache_test_matrix() {
    FlexibleSparseMatrix m;
    m.d1 = 2; m.d2 = 2; m.d3 = 1; m.d4 = 1; // 4 Knoten, 1x1 Blöcke
    m.knotenAnzahl = 4;
    m.B = 1;
    m.nne = 4; // Nur 4 Einträge für den Test
    m.eintraege = malloc(m.nne * sizeof(MatrixEintrag));

    // Beispiel-Matrix:
    // [ 1.0  2.0  0.0  0.0 ]
    // [ 0.0  3.0  0.0  0.0 ]
    // [ 4.0  0.0  5.0  0.0 ]
    // [ 0.0  0.0  0.0  6.0 ]
    m.eintraege[0] = (MatrixEintrag){0, 0, 1.0};
    m.eintraege[1] = (MatrixEintrag){0, 1, 2.0};
    m.eintraege[2] = (MatrixEintrag){1, 1, 3.0};
    m.eintraege[3] = (MatrixEintrag){2, 0, 4.0};
    // ... fülle alle, die du testen willst
    return m;
}

void test_csr_konvertierung() {
    FlexibleSparseMatrix testM = erzeuge_einfache_test_matrix();
    CSRMatrix csr = konvertiere_zu_csr(testM);

    // DEINE ERWARTUNGEN (Hier berechnest du manuell, was rauskommen muss)
    double expected_val[] = {1.0, 2.0, 3.0, 4.0};
    int expected_ci[]    = {0, 1, 1, 0};
    int expected_rst[]   = {0, 2, 3, 4, 4}; // Zeilenstart-Indizes

    // 1. Check nnz
    if (csr.nnz != 4) printf("FEHLER: nnz stimmt nicht!\n");

    // 2. Check Werte
    for (int k = 0; k < csr.nnz; k++) {
        if (fabs(csr.val[k] - expected_val[k]) > 1e-9) {
            printf("FEHLER: val[%d] ist falsch! Erwartet: %f, Ist: %f\n", k, expected_val[k], csr.val[k]);
        }
        if (csr.ci[k] != expected_ci[k]) {
            printf("FEHLER: ci[%d] ist falsch!\n", k);
        }
    }

    // 3. Check Row-Start
    for (int i = 0; i <= csr.N; i++) {
        if (csr.rst[i] != expected_rst[i]) {
            printf("FEHLER: rst[%d] ist falsch!\n", i);
        }
    }

    printf("Test 'test_csr_konvertierung' erfolgreich abgeschlossen.\n");

    // Aufräumen
    free(testM.eintraege);
    freigabe_csr_matrix(csr);
}


//
// Created by Nia on 16.07.26.
//



#include <stdlib.h>
#include "matrixstruktur.h"

DichteMatrix konvertiere_zu_dicht(FlexibleSparseMatrix sparse) {
    DichteMatrix dichteMatrix;
    // Die Dimension ist N * B!
    dichteMatrix.N = sparse.knotenAnzahl * sparse.B;

    dichteMatrix.daten = malloc(dichteMatrix.N * sizeof(double *));
    for (int i = 0; i < dichteMatrix.N; i++) {
        dichteMatrix.daten[i] = calloc(dichteMatrix.N, sizeof(double));
    }

    for (int k = 0; k < sparse.nne; k++) {
        int r = sparse.eintraege[k].i;
        int s = sparse.eintraege[k].j;

        // Sicherheitshalber prüfen, ob der Index im Bereich liegt
        if (r < dichteMatrix.N && s < dichteMatrix.N) {
            dichteMatrix.daten[r][s] = sparse.eintraege[k].wert;
        }
    }
    return dichteMatrix;

}




// Hilfsfunktion zum Sortieren der Einträge (für korrektes CSR)
int compare_eintraege(const void *a, const void *b) {
    MatrixEintrag *e1 = (MatrixEintrag *)a;
    MatrixEintrag *e2 = (MatrixEintrag *)b;
    if (e1->i != e2->i) return e1->i - e2->i;
    return e1->j - e2->j;
}

CSRMatrix konvertiere_zu_csr(FlexibleSparseMatrix sparse) {
    CSRMatrix csr;
    csr.N = sparse.knotenAnzahl * sparse.B;
    csr.nnz = sparse.nne; // Annahme: Alle Einträge sind vorhanden

    // 1. Sortiere Einträge nach Zeile, dann Spalte
    qsort(sparse.eintraege, sparse.nne, sizeof(MatrixEintrag), compare_eintraege);

    // 2. Speicher allokieren
    csr.val = malloc(csr.nnz * sizeof(double));
    csr.ci = malloc(csr.nnz * sizeof(int));
    csr.rst = calloc(csr.N + 1, sizeof(int)); // +1 für den End-Pointer der letzten Zeile

    // 3. Befüllen
    for (int k = 0; k < csr.nnz; k++) {
        csr.val[k] = sparse.eintraege[k].wert;
        csr.ci[k] = sparse.eintraege[k].j;
        // Inkrementiere den Zeilenzähler für jeden Eintrag
        csr.rst[sparse.eintraege[k].i + 1]++;
    }

    // 4. Kumulative Summe für rst (Row Start Pointer) bilden
    for (int i = 0; i < csr.N; i++) {
        csr.rst[i + 1] += csr.rst[i];
    }

    return csr;
}

void sortiere_sparse_matrix(FlexibleSparseMatrix *m) {
    qsort(m->eintraege, m->nne, sizeof(MatrixEintrag), compare_eintraege);
}



CSRMatrix konvertiere_zu_optimierten_csr(FlexibleSparseMatrix sparse) {
    CSRMatrix csr;
    csr.N = sparse.knotenAnzahl * sparse.B;
    int N = csr.N;
    int limit = N / 2; // Die Grenze für die Zonenteilung

    // 1. Sortieren
    sortiere_sparse_matrix(&sparse);

    // 2. Speicher-Struktur vorbereiten
    csr.rst = malloc((N + 1) * sizeof(int));
    csr.rst[0] = 0;
    int laufender_nnz = 0;

    // Wir brauchen ein temporäres Array, um zu wissen, wie viele Elemente wir pro Zeile
    // in Zone 1 (spärlich) haben, um die Indizes korrekt zu setzen.
    int* nnz_pro_zeile = calloc(N, sizeof(int));

    // Zone 1 (i < N/2): Nur vorhandene zählen
    for (int i = 0; i < limit; i++) {
        for (int k = 0; k < sparse.nne; k++) {
            if (sparse.eintraege[k].i == i && sparse.eintraege[k].j >= i) {
                nnz_pro_zeile[i]++;
            }
        }
        laufender_nnz += nnz_pro_zeile[i];
        csr.rst[i + 1] = laufender_nnz;
    }

    // Zone 2 (i >= N/2): Volle Kapazität für Fill-in
    for (int i = limit; i < N; i++) {
        nnz_pro_zeile[i] = (N - i);
        laufender_nnz += nnz_pro_zeile[i];
        csr.rst[i + 1] = laufender_nnz;
    }

    // 3. Jetzt erst Speicher für val und ci allokieren
    csr.nnz = laufender_nnz;
    csr.val = calloc(csr.nnz, sizeof(double));
    csr.ci = malloc(csr.nnz * sizeof(int));

    // 4. Struktur final befüllen
    for (int i = 0; i < N; i++) {
        int start = csr.rst[i];
        if (i < limit) {
            // Zone 1: Nur die existierenden Sparse-Elemente eintragen
            int p = start;
            for (int k = 0; k < sparse.nne; k++) {
                if (sparse.eintraege[k].i == i && sparse.eintraege[k].j >= i) {
                    csr.ci[p] = sparse.eintraege[k].j;
                    csr.val[p] = sparse.eintraege[k].wert;
                    p++;
                }
            }
        } else {
            // Zone 2: Komplettes Gitter befüllen
            for (int j = 0; j < (N - i); j++) {
                csr.ci[start + j] = i + j;
                // Werte werden hier bei Bedarf in einem zweiten Durchgang eingetragen
            }
        }
    }

    // 5. Werte für Zone 2 nachtragen (aus sparse)
    for (int k = 0; k < sparse.nne; k++) {
        int i = sparse.eintraege[k].i;
        int j = sparse.eintraege[k].j;
        if (i >= limit && j >= i) {
            for (int p = csr.rst[i]; p < csr.rst[i + 1]; p++) {
                if (csr.ci[p] == j) {
                    csr.val[p] = sparse.eintraege[k].wert;
                    break;
                }
            }
        }
    }

    free(nnz_pro_zeile);
    return csr;
}


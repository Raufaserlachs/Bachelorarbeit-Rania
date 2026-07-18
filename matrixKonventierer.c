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




// --- Konvertierung von FlexibleSparseMatrix zu BCSR ---
BCSRMatrix konvertiere_zu_bcsr(FlexibleSparseMatrix sparse) {
    BCSRMatrix bcsr;
    bcsr.N = sparse.knotenAnzahl; // Anzahl Blöcke
    bcsr.B = sparse.B;
    int B2 = bcsr.B * bcsr.B;
    int limit = bcsr.N / 2;

    // 1. Sortieren
    sortiere_sparse_matrix(&sparse);

    // 2. Speicher-Struktur vorbereiten
    bcsr.row_ptr = calloc(bcsr.N + 1, sizeof(int));

    // Wir zählen Blöcke (statt Einzelwerte)
    // Zone 1: Nur existierende Blöcke zählen
    // Zone 2: Alle Blöcke J >= I werden reserviert (für Fill-in)
    int laufender_block_idx = 0;
    for (int i = 0; i < bcsr.N; i++) {
        bcsr.row_ptr[i] = laufender_block_idx;
        if (i < limit) {
            // Zähle existierende Blöcke
            for (int k = 0; k < sparse.nne; k += B2) {
                if (sparse.eintraege[k].i / bcsr.B == i) {
                    laufender_block_idx++;
                }
            }
        } else {
            // Reserviere alles für Fill-in
            laufender_block_idx += (bcsr.N - i);
        }
    }
    bcsr.row_ptr[bcsr.N] = laufender_block_idx;

    // 3. Allokieren
    bcsr.val = calloc(laufender_block_idx * B2, sizeof(double));
    bcsr.col_idx = malloc(laufender_block_idx * sizeof(int));

    // 4. Daten füllen
    for (int i = 0; i < bcsr.N; i++) {
        int start = bcsr.row_ptr[i];
        if (i < limit) {
            int p = start;
            for (int k = 0; k < sparse.nne; k += B2) {
                if (sparse.eintraege[k].i / bcsr.B == i) {
                    bcsr.col_idx[p] = sparse.eintraege[k].j / bcsr.B;
                    // Kopiere den ganzen BxB Block
                    for(int b = 0; b < B2; b++) bcsr.val[p * B2 + b] = sparse.eintraege[k + b].wert;
                    p++;
                }
            }
        } else {
            for (int j = 0; j < (bcsr.N - i); j++) {
                bcsr.col_idx[start + j] = i + j;
            }
            // Nachtragen der bekannten Werte in Zone 2
            for (int k = 0; k < sparse.nne; k += B2) {
                int block_i = sparse.eintraege[k].i / bcsr.B;
                int block_j = sparse.eintraege[k].j / bcsr.B;
                if (block_i == i && block_j >= i) {
                    for (int p = start; p < bcsr.row_ptr[i + 1]; p++) {
                        if (bcsr.col_idx[p] == block_j) {
                            for(int b = 0; b < B2; b++) bcsr.val[p * B2 + b] = sparse.eintraege[k + b].wert;
                            break;
                        }
                    }
                }
            }
        }
    }
    return bcsr;
}
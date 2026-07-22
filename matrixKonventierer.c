//
// Created by Nia on 16.07.26.
//


#include <stdlib.h>
#include <string.h>
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



//Aufgabe: Vorhandene Einträge der Sparse-Matrix sortieren
void sortiere_sparse_matrix(FlexibleSparseMatrix *m) {
    qsort(m->eintraege, m->nne, sizeof(MatrixEintrag), compare_eintraege);
}

// Speicherbedarf pro Zeile berechnen (unterteilt in Zone 1 und Zone 2)
int* berechne_nnz_pro_zeile(int N, int limit, FlexibleSparseMatrix sparse, int *laufender_nnz_out) {
    int *nnz_pro_zeile = calloc(N, sizeof(int));
    int laufender_nnz = 0;

    // Zone 1: Nur echte vorhandene Einträge zählen
    for (int i = 0; i < limit; i++) {
        for (int k = 0; k < sparse.nne; k++) {
            if (sparse.eintraege[k].i == i) {
                nnz_pro_zeile[i]++;
            }
        }
        laufender_nnz += nnz_pro_zeile[i];
    }

    // Zone 2 (ab 4. Quadrant): Volle Zeile erzwingen
    for (int i = limit; i < N; i++) {
        nnz_pro_zeile[i] = N;
        laufender_nnz += N;
    }

    *laufender_nnz_out = laufender_nnz;
    return nnz_pro_zeile;
}


// CSR-Struktur und Grundspeicher allokieren
CSRMatrix allokiere_csr_struktur(int N, int nnz, int *nnz_pro_zeile) {
    CSRMatrix csr;
    csr.N = N;
    csr.nnz = nnz;
    csr.rst = malloc((N + 1) * sizeof(int));
    csr.val = calloc(nnz, sizeof(double));
    csr.ci = malloc(nnz * sizeof(int));

    // Row-Start (rst) Array aufbauen
    csr.rst[0] = 0;
    for (int i = 0; i < N; i++) {
        csr.rst[i + 1] = csr.rst[i] + nnz_pro_zeile[i];
    }

    return csr;
}


// Spaltenindizes (ci) für beide Zonen eintragen
void fuelle_spaltenindizes(CSRMatrix *csr, int N, int limit, FlexibleSparseMatrix sparse) {
    for (int i = 0; i < N; i++) {
        int start = csr->rst[i];
        if (i < limit) {
            // Zone 1: Nur echte Spaltenindizes eintragen
            int p = start;
            for (int k = 0; k < sparse.nne; k++) {
                if (sparse.eintraege[k].i == i) {
                    csr->ci[p] = sparse.eintraege[k].j;
                    p++;
                }
            }
        } else {
            // Zone 2 (4. Quadrant): Alle Spalten von 0 bis N-1 indizieren
            for (int j = 0; j < N; j++) {
                csr->ci[start + j] = j;
            }
        }
    }
}

// Werte aus der Sparse-Matrix in das CSR-val-Array übertragen
void fuelle_werte(CSRMatrix *csr, FlexibleSparseMatrix sparse) {
    for (int k = 0; k < sparse.nne; k++) {
        int i = sparse.eintraege[k].i;
        int j = sparse.eintraege[k].j;
        double wert = sparse.eintraege[k].wert;

        // Finde den passenden Platz in der Zeile i
        for (int p = csr->rst[i]; p < csr->rst[i + 1]; p++) {
            if (csr->ci[p] == j) {
                csr->val[p] = wert;
                break;
            }
        }
    }
}





// Koordinierungsfunktion für csr konvertierung
CSRMatrix konvertiere_zu_optimierten_csr(FlexibleSparseMatrix sparse) {

    int N = sparse.knotenAnzahl * sparse.B;
    int limit = N / 2;

    // 1. Sortieren
    sortiere_sparse_matrix(&sparse);

    // 2. NNZ pro Zeile berechnen
    int laufender_nnz = 0;
    int *nnz_pro_zeile = berechne_nnz_pro_zeile(N, limit, sparse, &laufender_nnz);

    // 3. Speicher allokieren & rst aufbauen
    CSRMatrix csr = allokiere_csr_struktur(N, laufender_nnz, nnz_pro_zeile);

    // 4. Spaltenindizes (ci) befüllen
    fuelle_spaltenindizes(&csr, N, limit, sparse);

    // 5. Werte (val) übertragen
    fuelle_werte(&csr, sparse);

    free(nnz_pro_zeile);
    return csr;
}
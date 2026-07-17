//
// Created by Nia on 16.07.26.
//



#include <stdlib.h>
#include "Matrixstruktur.h"

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

    // 1. Sortieren
    sortiere_sparse_matrix(&sparse);

    // 2. Speicherbedarf für das obere Dreieck (Gauß-Struktur)
    // Ein statisches CSR für das obere Dreieck hat bei N Zeilen:
    // Zeile i hat (csr.N - i) Elemente.
    long n = (long)csr.N;
    csr.nnz = (int)((n * (n + 1)) / 2);

    // 3. Speicher allokieren
    csr.val = calloc(csr.nnz, sizeof(double)); // Alle Werte initial 0.0
    csr.ci = malloc(csr.nnz * sizeof(int));
    csr.rst = malloc((csr.N + 1) * sizeof(int));

    // 4. Struktur komplett vorinitialisieren (das ist der entscheidende Schritt)
    csr.rst[0] = 0;
    for (int i = 0; i < csr.N; i++) {
        // Zeile i hat (csr.N - i) Einträge
        csr.rst[i + 1] = csr.rst[i] + (csr.N - i);

        // Spalten-Indizes für diese Zeile festlegen
        int start = csr.rst[i];
        for (int j = 0; j < (csr.N - i); j++) {
            csr.ci[start + j] = i + j;
        }
    }

    // 5. Werte aus der SparseMatrix in die statische Struktur übertragen
    for (int k = 0; k < sparse.nne; k++) {
        int i = sparse.eintraege[k].i;
        int j = sparse.eintraege[k].j;

        if (j >= i) { // Nur oberes Dreieck
            // Suche den Platz in der Zeile i
            for (int p = csr.rst[i]; p < csr.rst[i + 1]; p++) {
                if (csr.ci[p] == j) {
                    csr.val[p] = sparse.eintraege[k].wert;
                    break;
                }
            }
        }
    }

    return csr;
}


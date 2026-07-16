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

    // 1. Sortieren (Fundament für CSR)
    sortiere_sparse_matrix(&sparse);

    // 2. Speicher-Bedarf DEFINITIV berechnen
    // A: Die obere Dreiecksmatrix theoretisch (für eine DichteMatrix wäre es 0.5 * N^2)
    //    Wir nehmen den Worst-Case für die Obere Dreiecksmatrix an.
    long n_long = (long)csr.N;
    long puffer_fillin = (n_long * (n_long + 1)) / 2; // Gaußsche Summenformel für Obere Dreiecksmatrix

    // Wir setzen nnz fest auf die volle Kapazität der oberen Dreiecksmatrix
    csr.nnz = (int)puffer_fillin;

    // 3. Speicher allokieren (Alles auf 0.0 initialisiert)
    csr.val = calloc(csr.nnz, sizeof(double));
    csr.ci = malloc(csr.nnz * sizeof(int));
    csr.rst = calloc(csr.N + 1, sizeof(int));

    // 4. Befüllen mit den existierenden Werten
    // WICHTIG: Da wir jetzt eine statische Struktur haben, müssen wir beim
    // Befüllen darauf achten, dass die rst-Struktur auch wirklich die
    // Puffer-Plätze mitzählt!

    for (int k = 0; k < sparse.nne; k++) {
        if (sparse.eintraege[k].j >= sparse.eintraege[k].i) {
            // HIER liegt die Gefahr: Wenn du nur die existierenden einträgst,
            // ist dein rst-Pointer "dünn", aber dein Speicher ist "fett".
            // Für den Gauß-Solver ist das okay, solange du die Puffer-Plätze
            // beim Update als "gefüllt" markierst.

            csr.val[k] = sparse.eintraege[k].wert;
            csr.ci[k] = sparse.eintraege[k].j;
            csr.rst[sparse.eintraege[k].i + 1]++;
        }
    }

    // 5. Kumulative Summe für rst
    for (int i = 0; i < csr.N; i++) {
        csr.rst[i + 1] += csr.rst[i];
    }

    return csr;
}




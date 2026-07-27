//
// Created by Nia on 16.07.26.
//


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "matrixstruktur.h"
#include <stdbool.h>



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







// Aufgabe: Vorhandene Einträge der Sparse-Matrix sortieren
void sortiere_sparse_matrix(FlexibleSparseMatrix *m) {
    qsort(m->eintraege, m->nne, sizeof(MatrixEintrag), compare_eintraege);
}

// Hilfsstruktur für die blockbasierte symbolische Analyse (Einfaches dynamisches Array pro Block-Zeile)
typedef struct {
    int *spalten;
    int anzahl;
    int kapazitaet;
} BlockZeile;

void fuege_eindeutig_hinzu(BlockZeile *bz, int col) {
    for (int i = 0; i < bz->anzahl; i++) {
        if (bz->spalten[i] == col) return; // Schon da
    }
    if (bz->anzahl >= bz->kapazitaet) {
        bz->kapazitaet = bz->kapazitaet == 0 ? 4 : bz->kapazitaet * 2;
        bz->spalten = realloc(bz->spalten, bz->kapazitaet * sizeof(int));
    }
    bz->spalten[bz->anzahl++] = col;
}

bool enthalt_block(BlockZeile *bz, int col) {
    for (int i = 0; i < bz->anzahl; i++) {
        if (bz->spalten[i] == col) return true;
    }
    return false;
}

// Block-basierte symbolische Faktorisierung & exakte NNZ-Bestimmung pro skalarer Zeile
int* berechne_nnz_pro_zeile_symbolisch(int N, int B, FlexibleSparseMatrix sparse, int *laufender_nnz_out) {
    int K = sparse.knotenAnzahl; // Anzahl der Blöcke

    // 1. Block-Strukturen für jede Block-Zeile initialisieren
    BlockZeile *block_struktur = malloc(K * sizeof(BlockZeile));
    for (int i = 0; i < K; i++) {
        block_struktur[i].anzahl = 0;
        block_struktur[i].kapazitaet = 0;
        block_struktur[i].spalten = NULL;
    }

    // Initiale Nachbarn aus der Sparse-Matrix eintragen
    for (int k = 0; k < sparse.nne; k++) {
        int block_i = sparse.eintraege[k].i / B;
        int block_j = sparse.eintraege[k].j / B;
        fuege_eindeutig_hinzu(&block_struktur[block_i], block_j);
    }

    // 2. Symbolische Right-Looking Elimination auf Block-Ebene simulieren
    for (int i = 0; i < K - 1; i++) {
        // Für jede Block-Spalte j in Zeile i (mit j > i)
        for (int idx_j = 0; idx_j < block_struktur[i].anzahl; idx_j++) {
            int j = block_struktur[i].spalten[idx_j];
            if (j > i) {
                // Wenn Block (j, i) existiert, breitet sich das Fill-in aus
                if (enthalt_block(&block_struktur[j], i)) {
                    for (int idx_k = 0; idx_k < block_struktur[i].anzahl; idx_k++) {
                        int k = block_struktur[i].spalten[idx_k];
                        if (k > i) {
                            fuege_eindeutig_hinzu(&block_struktur[j], k);
                        }
                    }
                }
            }
        }
    }

    // 3. Skalare NNZ pro Zeile anhand der finalen Block-Struktur berechnen
    int *nnz_pro_zeile = calloc(N, sizeof(int));
    int laufender_nnz = 0;

    for (int skalar_i = 0; skalar_i < N; skalar_i++) {
        int block_i = skalar_i / B;
        // Jede Block-Spalte in der Block-Zeile bringt B skalare Einträge mit
        int nnz_hier = block_struktur[block_i].anzahl * B;
        nnz_pro_zeile[skalar_i] = nnz_hier;
        laufender_nnz += nnz_hier;
    }

    // Aufräumen der temporären Block-Strukturen
    for (int i = 0; i < K; i++) {
        free(block_struktur[i].spalten);
    }
    free(block_struktur);

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

// Spaltenindizes (ci) über die symbolisch ermittelte Block-Struktur eintragen
void fuelle_spaltenindizes_symbolisch(CSRMatrix *csr, int N, int B, FlexibleSparseMatrix sparse) {
    int K = sparse.knotenAnzahl;

    // Wir bauen die Block-Struktur für das Eintragen noch einmal kurz auf (oder übergeben sie direkt)
    BlockZeile *block_struktur = malloc(K * sizeof(BlockZeile));
    for (int i = 0; i < K; i++) {
        block_struktur[i].anzahl = 0;
        block_struktur[i].kapazitaet = 0;
        block_struktur[i].spalten = NULL;
    }
    for (int k = 0; k < sparse.nne; k++) {
        int block_i = sparse.eintraege[k].i / B;
        int block_j = sparse.eintraege[k].j / B;
        fuege_eindeutig_hinzu(&block_struktur[block_i], block_j);
    }
    for (int i = 0; i < K - 1; i++) {
        for (int idx_j = 0; idx_j < block_struktur[i].anzahl; idx_j++) {
            int j = block_struktur[i].spalten[idx_j];
            if (j > i && enthalt_block(&block_struktur[j], i)) {
                for (int idx_k = 0; idx_k < block_struktur[i].anzahl; idx_k++) {
                    int k = block_struktur[i].spalten[idx_k];
                    if (k > i) fuege_eindeutig_hinzu(&block_struktur[j], k);
                }
            }
        }
    }

    // Spaltenindizes ins ci-Array schreiben
    for (int skalar_i = 0; skalar_i < N; skalar_i++) {
        int block_i = skalar_i / B;
        int p = csr->rst[skalar_i];

        for (int idx_j = 0; idx_j < block_struktur[block_i].anzahl; idx_j++) {
            int block_j = block_struktur[block_i].spalten[idx_j];
            for (int sub_c = 0; sub_c < B; sub_c++) {
                csr->ci[p++] = (block_j * B) + sub_c;
            }
        }
    }

    for (int i = 0; i < K; i++) free(block_struktur[i].spalten);
    free(block_struktur);
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

// Koordinierungsfunktion für die optimierte CSR-Konvertierung mit symbolischer Faktorisierung
CSRMatrix konvertiere_zu_optimierten_csr(FlexibleSparseMatrix sparse) {
    int N = sparse.knotenAnzahl * sparse.B;
    int B = sparse.B;

    // 1. Sortieren
    sortiere_sparse_matrix(&sparse);

    // 2. NNZ pro Zeile durch symbolische Block-Faktorisierung bestimmen
    int laufender_nnz = 0;
    int *nnz_pro_zeile = berechne_nnz_pro_zeile_symbolisch(N, B, sparse, &laufender_nnz);

    // 3. Speicher allokieren & rst aufbauen
    CSRMatrix csr = allokiere_csr_struktur(N, laufender_nnz, nnz_pro_zeile);

    // 4. Spaltenindizes (ci) symbolisch befüllen
    fuelle_spaltenindizes_symbolisch(&csr, N, B, sparse);

    // 5. Werte (val) übertragen
    fuelle_werte(&csr, sparse);

    free(nnz_pro_zeile);
    return csr;
}
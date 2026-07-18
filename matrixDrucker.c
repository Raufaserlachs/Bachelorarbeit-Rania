//
// Created by Nia on 16.07.26.
//

#include <stdio.h>
#include <math.h>
#include "matrixstruktur.h"




void drucke_flexible_matrix_tabelle(FlexibleSparseMatrix m) {
    // Knotenanahl für x y matrix
    int n = m.knotenAnzahl;

    // //  Nur drucken, wenn die Matrix nicht zu riesig ist
    // if (n > 100) {
    //     printf("\n Matrix ist mit zu groß für die Tabellenansicht! ---\n");
    //     return;
    // }



    // Spaltennummerierung oben
    printf("     ");
    for(int j = 0; j < n; j++) printf("  %2d  ", j);
    printf("\n    -");
    for(int j = 0; j < n; j++) printf("---");
    printf("\n");

    //nachbar check falsch für matrizen mit d =1
    for (int i = 0; i < n; i++) {
        printf("%2d | ", i); // Zeilennummer am Rand

        for (int j = 0; j < n; j++) {
            int hatNachbar = 0;
            double wertGefunden = 0;

            // suche nur nach Einträgen, die zur aktuellen Zeile i gehören
            for (int k = 0; k < m.nne; k++) {

                // wenn Eintrag für Zeile i und Spalte j gefunden
                if (m.eintraege[k].i == i && m.eintraege[k].j == j) {
                    hatNachbar = 1;
                    wertGefunden = m.eintraege[k].wert; // Wert einlesen/merken
                    break;
                }
                // wenn nächsten Zeileneinträge erreicht,  abbrechen
                if (m.eintraege[k].i > i) break;
            }

            if (hatNachbar) printf(" %5.2f " , wertGefunden ); else printf("  .   ");
        }
        printf("\n");
    }
    printf("------------------------------------------\n");

}



void drucke_dichte_matrix(DichteMatrix dichteMatrix) {
    printf("\n--- Matrix (%dx%d) ---\n", dichteMatrix.N, dichteMatrix.N);

    // Spaltennummern oben drucken
    printf("     ");
    for (int j = 0; j < dichteMatrix.N; j++) {
        // %6d mit zwei Leerzeichen drumherum =  8 Zeichen Breite
        printf("  [%2d]  ", j);
    }
    printf("\n     ");
    for (int j = 0; j < dichteMatrix.N; j++) {
        printf("--------"); // 8 Bindestriche pro Spalte
    }
    printf("\n");

    // Schwellenwert für numerische Nullen alles kleiner ist quasi ein rundungsfehler
    double epsilon = 1e-12;

    // Zeilen durchlaufen
    for (int i = 0; i < dichteMatrix.N; i++) {
        printf("%2d | ", i);

        for (int j = 0; j < dichteMatrix.N; j++) {
            if (fabs(dichteMatrix.daten[i][j]) < epsilon) {
                // Exakt 8 Zeichen breit: 4 Leerzeichen, Punkt, 3 Leerzeichen
                printf("    .   ");
            }
            else {
                // %8.2f reserviert fest 8 Zeichen Gesamtbreite.
                // Das Vorzeichen '-' wird innerhalb dieser 8 Zeichen platziert.
                printf("%8.2f", dichteMatrix.daten[i][j]);
            }
        }
        printf("\n");
    }
    printf("--------------------------------------------------------------------------------\n\n");
}



void drucke_csr_matrix(CSRMatrix A) {
    printf("\n--- CSR Matrix (%dx%d) ---\n", A.N, A.N);

    // Header
    printf("     ");
    for (int j = 0; j < A.N; j++) printf("  [%2d]  ", j);
    printf("\n     ");
    for (int j = 0; j < A.N; j++) printf("--------");
    printf("\n");

    // Zeilenweise durchlaufen
    for (int i = 0; i < A.N; i++) {
        printf("%2d | ", i);

        // Suche in der gesamten Zeile nach Spalte j
        for (int j = 0; j < A.N; j++) {
            double wert = 0.0;
            int gefunden = 0;

            // Durchsuche nur den Bereich dieser Zeile in CSR
            for (int k = A.rst[i]; k < A.rst[i+1]; k++) {
                if (A.ci[k] == j) {
                    wert = A.val[k];
                    gefunden = 1;
                    break;
                }
            }

            if (gefunden) {
                printf("%8.2f", wert);
            } else {
                printf("    .   ");
            }
        }
        printf("\n");
    }
}



void drucke_optimierte_csr(CSRMatrix A) {
    printf("\n--- Optimierte CSR-Matrix (Symmetrisch gespiegelt) ---\n");

    // Header
    printf("     ");
    for (int j = 0; j < A.N; j++) printf("  [%2d]  ", j);
    printf("\n     ");
    for (int j = 0; j < A.N; j++) printf("--------");
    printf("\n");

    for (int i = 0; i < A.N; i++) {
        printf("%2d | ", i);

        for (int j = 0; j < A.N; j++) {
            double wert = 0.0;

            // Logik:
            // 1. Wenn j >= i: Suche in CSR (Oberes Dreieck)
            // 2. Wenn j < i: Suche den Wert A[j][i] (Unteres Dreieck -> gespiegelt)

            int row = (j >= i) ? i : j;
            int col = (j >= i) ? j : i;

            // Suche in der entsprechenden Zeile 'row' nach Spalte 'col'
            for (int k = A.rst[row]; k < A.rst[row + 1]; k++) {
                if (A.ci[k] == col) {
                    wert = A.val[k];
                    break;
                }
            }

            if (wert != 0.0 )
                printf("%8.2f", wert);
            else
                printf("    .   ");
        }
        printf("\n");
    }
}



void drucke_csr_nach_gauss(CSRMatrix A) {
    printf("\n--- CSR-Matrix nach Gauß-Elimination (Obere Dreiecksform) ---\n");

    for (int i = 0; i < A.N; i++) {
        printf("%2d | ", i);

        for (int j = 0; j < A.N; j++) {
            // Wir suchen nur noch im oberen Dreieck (und Diagonale)
            if (j >= i) {
                double wert = 0.0;
                int gefunden = 0;

                // Suche in der Zeile i
                for (int k = A.rst[i]; k < A.rst[i + 1]; k++) {
                    if (A.ci[k] == j) {
                        wert = A.val[k];
                        gefunden = 1;
                        break;
                    }
                }

                if (gefunden)
                    printf("%8.2f", wert);
                else
                    printf("    .   "); // Punkt für fehlende Werte im oberen Dreieck
            } else {
                // Unterhalb der Diagonale: Explizit den Punkt drucken, statt "0.00"
                printf("    .   ");
            }
        }
        printf("\n");
    }
}






/**
 * Druckt die BCSR-Matrix vor Gauß (unter Berücksichtigung der Symmetrie).
 */
void drucke_bcsr_vor_gauss(BCSRMatrix A) {
    printf("\n--- BCSR-Matrix vor Gauß (Rekonstruktion aus oberem Dreieck) ---\n");

    for (int i = 0; i < A.N; i++) {
        for (int row_in_block = 0; row_in_block < A.B; row_in_block++) {
            printf("%2d | ", i * A.B + row_in_block);

            for (int j = 0; j < A.N; j++) {

                // Wir suchen den Block A[i][j]
                // Bei Symmetrie: Wenn wir A[i][j] brauchen, suchen wir min(i,j), max(i,j)
                int row_idx = (i < j) ? i : j;
                int col_idx = (i < j) ? j : i;

                double *block_val = NULL;
                for (int p = A.row_ptr[row_idx]; p < A.row_ptr[row_idx + 1]; p++) {
                    if (A.col_idx[p] == col_idx) {
                        block_val = &A.val[p * A.B * A.B];
                        break;
                    }
                }

                // Drucke die Werte
                for (int col_in_block = 0; col_in_block < A.B; col_in_block++) {
                    if (block_val != NULL) {
                        // Bei Transponierung (j < i): Indizes tauschen
                        int r = (i < j) ? row_in_block : col_in_block;
                        int c = (i < j) ? col_in_block : row_in_block;
                        printf("%8.2f", block_val[r * A.B + c]);
                    } else {
                        printf("    .   ");
                    }
                }
                printf(" ");
            }
            printf("\n");
        }
        printf("------------------------------------------------------------\n");
    }
}














void drucke_bcsr_nach_gauss(BCSRMatrix A) {
    printf("\n--- BCSR-Matrix nach Gauß-Elimination (Obere Block-Dreiecksform) ---\n");

    // Über jeden Block-Zeilen-Index
    for (int i = 0; i < A.N; i++) {
        // Über jede Zeile innerhalb des Blocks
        for (int row_in_block = 0; row_in_block < A.B; row_in_block++) {
            printf("%2d | ", i * A.B + row_in_block);

            // Über jeden Block-Spalten-Index
            for (int j = 0; j < A.N; j++) {

                // Wir suchen den Block A[i][j]
                double *block_val = NULL;
                for (int p = A.row_ptr[i]; p < A.row_ptr[i + 1]; p++) {
                    if (A.col_idx[p] == j) {
                        block_val = &A.val[p * A.B * A.B];
                        break;
                    }
                }

                // Drucke die Werte innerhalb des Blocks
                for (int col_in_block = 0; col_in_block < A.B; col_in_block++) {
                    // Logik: Unterhalb der Hauptdiagonale (Block-Ebene: j < i) -> Punkt
                    // Ansonsten: Wert drucken (wenn vorhanden) oder Punkt
                    if (j < i) {
                        printf("    .   ");
                    } else if (block_val != NULL) {
                        printf("%8.2f", block_val[row_in_block * A.B + col_in_block]);
                    } else {
                        printf("    .   ");
                    }
                }
                // Kleiner Abstand zwischen den Blöcken
                printf(" ");
            }
            printf("\n");
        }
        // Trennlinie nach jedem Block (optisch schöner)
        printf("------------------------------------------------------------\n");
    }
}







// void drucke_dichte_matrix(DichteMatrix dichteMatrix) {
//     printf("--- Matrix (%dx%d) ---\n", dichteMatrix.N, dichteMatrix.N);
//
//     for (int i = 0; i < dichteMatrix.N; i++) {
//         for (int j = 0; j < dichteMatrix.N; j++) {
//             if (dichteMatrix.daten[i][j] == 0) {
//                 printf("   X  ");
//             }
//             else printf("%7.2f ", dichteMatrix.daten[i][j]);
//         }
//         printf("\n");
//     }
//     printf("---------------------\n");
//
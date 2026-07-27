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

            // Durchsuche den Bereich dieser Zeile in CSR
            for (int k = A.rst[i]; k < A.rst[i+1]; k++) {
                if (A.ci[k] == j) {
                    wert = A.val[k];
                    gefunden = 1;
                    break;
                }
            }

            // Wenn der Wert gefunden wurde UND ungleich 0.0 ist, drucke ihn.
            // Ansonsten (also wenn nicht gefunden ODER der Wert exakt 0.0 ist) drucke den Punkt.
            if (gefunden && wert != 0.0) {
                printf("%8.2f", wert);
            } else {
                printf("    .   ");
            }
        }
        printf("\n");
    }
}



/**
 * Gibt den Lösungsvektor x formatiert aus.
 */
void drucke_vektor_x(double x[], int N) {
    printf("Lösungsvektor x:\n");
    for (int i = 0; i < N; i++) {
        printf("x[%d] = %10.6f\n", i, x[i]);
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
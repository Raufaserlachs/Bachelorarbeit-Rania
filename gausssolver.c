//
// Created by Nia on 15.05.26.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>   // Für Zufallszahlen
#include <math.h>
#include "Matrixstruktur.h"


DichteMatrix konvertiere_zu_dicht(FlexibleSparseMatrix sparse) {

    DichteMatrix dichteMatrix;
    dichteMatrix.N = sparse.knotenAnzahl;


    // Speicher für Zeilen-Pointer reservieren
    dichteMatrix.daten = malloc(dichteMatrix.N * sizeof(double *));

    // Speicher für jede einzelne Zeile reservieren
    for (int i = 0; i < dichteMatrix.N; i++) {

        // calloc holt speicher und überschreibt  alles mit 0
        dichteMatrix.daten[i] = calloc(dichteMatrix.N, sizeof(double));
    }

    //NNE Werte aus der Sparse-Matrix übertragen
    for (int k = 0; k < sparse.nne; k++) {
        int r = sparse.eintraege[k].i; // Reihe
        int s = sparse.eintraege[k].j; // Spalte
        dichteMatrix.daten[r][s] = sparse.eintraege[k].wert;
    }

    return dichteMatrix;

}

//Funktion für reine ZSF also Gauss vorwärts
void bringe_in_zeilenstufenform(DichteMatrix dichteMatrix) {

    //i Läuft über pivot zeile
    for (int i = 0; i < dichteMatrix.N-1; i++) {
        for (int j = i+1; j < dichteMatrix.N; j++) {

            // Wie oft pivot wert in den zielwert passt
            double faktor = dichteMatrix.daten[j][i] / dichteMatrix.daten[i][i];

            //Subtraktion der oberen zeile k mit i
            //bei j=k starten weil links sowieso alles 0
            for (int k = i; k < dichteMatrix.N; k++) {
                dichteMatrix.daten[j][k] -= dichteMatrix.daten[i][k]*faktor;
            }

        }

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
// }

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
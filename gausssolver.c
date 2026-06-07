//
// Created by Nia on 15.05.26.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "Matrixstruktur.h"


DichteMatrix konvertiere_zu_dicht(FlexibleSparseMatrix sparse) {

    DichteMatrix dichteMatrix;
    dichteMatrix.N = sparse.knotenAnzahl;


    // Speicher für Zeilen-Pointer reservieren
    dichteMatrix.daten = malloc(dichteMatrix.N * sizeof(double *));

    // Speicher für jede einzelne Zeile reservieren
    for (int i = 0; i < dichteMatrix.N; i++) {

        // calloc holt Speicher und überschreibt  alles mit 0
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

//Änderung: Transformationsvektor
//Funktion für reine ZSF also Gauss vorwärts
void bringe_in_zeilenstufenform(DichteMatrix dichteMatrix, double b[]) {

    //i Läuft über pivot zeile
    for (int i = 0; i < dichteMatrix.N-1; i++) {
        for (int j = i+1; j < dichteMatrix.N; j++) {

            // Wie oft pivot wert in den zielwert passt
            double faktor = dichteMatrix.daten[j][i] / dichteMatrix.daten[i][i];

            b[j] -= b[i] * faktor;

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
//

// MMatrix original originalkopie
DichteMatrix kopiere_matrix(DichteMatrix A) {
    DichteMatrix original_kopie;
    original_kopie.N = A.N;
    original_kopie.daten = malloc(original_kopie.N * sizeof(double *));
    for (int i = 0; i < original_kopie.N; i++) {
        original_kopie.daten[i] = malloc(original_kopie.N * sizeof(double));
        for (int j = 0; j < original_kopie.N; j++) {
            original_kopie.daten[i][j] = A.daten[i][j];
        }
    }
    return original_kopie;
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


//pseudo
// FÜR jede Zeile i von der letzten bis zur ersten:
//     Nimm den Wert auf der rechten Seite (b[i])
//
//     ZIEHE alles ab, was wir schon wissen:
//         FÜR jede Spalte j rechts von der Diagonale:
//             Subtrahiere (Matrixwert * bereits bekanntes x[j])
//
//     TEILE den Rest durch das Diagonalelement (Matrixwert bei A[i][i])
//     Speichere das Ergebnis als x[i]

//Rücksubstitution
void loese_mit_ruecksubstitution(DichteMatrix A, double b[], double x[]) {

    // A ist bereits in Zeilenstufenform!
    int N = A.N;

    // Wir laufen von der letzten Zeile rückwärts bis zur ersten Zeile .
    // weil  wir in der letzten Zeile mit nur einer Unbekannten starten
    for (int i = N - 1; i >= 0; i--) {

        // Diese Variable sammelt die Produkte der bereits berechneten x-Werte
        // mit den entsprechenden Matrix-Koeffizienten der aktuellen Zeile i.
        double summe = 0.0;
        // Summiere die bereits bekannten x-Werte auf
        for (int j = i + 1; j < N; j++) {
            summe += A.daten[i][j] * x[j];
        }
        // x[i] = (b[i] - Summe) / Diagonalelement
        x[i] = (b[i] - summe) / A.daten[i][i];
    }



}



//Speicherpflege
void freigabe_dichte_matrix(DichteMatrix A) {
    // 1. Erst die einzelnen Zeilen freigeben
    for (int i = 0; i < A.N; i++) {
        free(A.daten[i]);
    }
    // 2. Dann den "Haupt-Zeiger" für die Zeilen-Pointer freigeben
    free(A.daten);
}


//gen mat ( d1 d2 d3 d4 und k) für bblocke
//mit RB formAT
//DÜNNBESETZER GAUSS ..... wo nicht null elemente und dannhandlung nur wo nicht null auch in zukunft

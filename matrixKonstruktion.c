#include <stdio.h>
#include <stdlib.h>
#include "Matrixstruktur.h"

SparseMatrix konstruiere_matrix() {

    SparseMatrix matrix;
    matrix.n = 16;
    matrix.nne = 80;

    //Speicher resverieren für Werte
    matrix.wert = (double *)malloc(matrix.nne * sizeof(double));
    //Speicher für 80 Spalten-Indizes
    matrix.spaltenIndex = (int *)malloc(matrix.nne * sizeof(int));
    // Speicher für n + 1 Zeilen-Pointer (16 + 1 = 17)
    matrix.zeilenIndex = (int *)malloc((matrix.n + 1) * sizeof(int));


    int zähler = 0;
    //Block 0-3
    // ZEILE 0
    matrix.zeilenIndex[0] = zähler;

    matrix.wert[zähler] = 1;
    matrix.spaltenIndex[zähler] = 0; zähler++;

    matrix.wert[zähler] = 1;
    matrix.spaltenIndex[zähler] = 1; zähler++;

    matrix.wert[zähler] = 1;
    matrix.spaltenIndex[zähler] = 3; zähler++;

    // matrix.wert[zähler] = 1;
    // matrix.spaltenIndex[zähler] = 4; zähler++;
    //
    // matrix.wert[zähler] = 1;
    // matrix.spaltenIndex[zähler] = 12; zähler++;

    // ZEILE 1
    matrix.zeilenIndex[1] = zähler;

    matrix.wert[zähler] = 1;
    matrix.spaltenIndex[zähler] = 0; zähler++;

    matrix.wert[zähler] = 1;
    matrix.spaltenIndex[zähler] = 1; zähler++;

    matrix.wert[zähler] = 1;
    matrix.spaltenIndex[zähler] = 2; zähler++;

    // matrix.wert[zähler] = 1;
    // matrix.spaltenIndex[zähler] = 5; zähler++;
    //
    // matrix.wert[zähler] = 1;
    // matrix.spaltenIndex[zähler] = 13; zähler++;

    // ZEILE 2
    matrix.zeilenIndex[2] = zähler;

    matrix.wert[zähler] = 1;
    matrix.spaltenIndex[zähler] = 1; zähler++;

    matrix.wert[zähler] = 1;
    matrix.spaltenIndex[zähler] = 2; zähler++;

    matrix.wert[zähler] = 1;
    matrix.spaltenIndex[zähler] = 3; zähler++;

    // matrix.wert[zähler] = 1;
    // matrix.spaltenIndex[zähler] = 6; zähler++;
    //
    // matrix.wert[zähler] = 1;
    // matrix.spaltenIndex[zähler] = 14; zähler++;


    // ZEILE 3
    matrix.zeilenIndex[3] = zähler;

    matrix.wert[zähler] = 1;
    matrix.spaltenIndex[zähler] = 0; zähler++;

    matrix.wert[zähler] = 1;
    matrix.spaltenIndex[zähler] = 2; zähler++;

    matrix.wert[zähler] = 1;
    matrix.spaltenIndex[zähler] = 3; zähler++;

    // matrix.wert[zähler] = 1;
    // matrix.spaltenIndex[zähler] = 7; zähler++;
    //
    // matrix.wert[zähler] = 1;
    // matrix.spaltenIndex[zähler] = 15; zähler++;


    //  markiert Ende von Zeile 3 markieren!
    matrix.zeilenIndex[4] = zähler;
    // Für den Test füllen wir die restlichen row_ptr Einträge (4 bis 16)
    // einfach mit dem aktuellen Zähler, damit das Programm nicht abstürzt.
    for(int i = 5; i <= 16; i++) {
        matrix.zeilenIndex[i] = zähler;
    }

    return matrix;
}

void print_matrix(SparseMatrix matrix) {
    printf("\n---  Matrix (Erste 4x4 Knoten) ---\n");
    for (int i = 0; i < 4; i++) {
        printf("Zeile %d: ", i);

        int start = matrix.zeilenIndex[i];
        int ende = matrix.zeilenIndex[i+1];

        for (int k = start; k < ende; k++) {
            printf("[%d: %.1f] ", matrix.spaltenIndex[k], matrix.wert[k]);
        }
        printf("\n");
    }
    printf("-----------------------------------------\n");
}


void loesche_matrix(SparseMatrix *m) {
    if (m->wert != NULL) free(m->wert);
    if (m->zeilenIndex != NULL) free(m->zeilenIndex);
    if (m->spaltenIndex != NULL) free(m->spaltenIndex);
    m->wert = NULL;
    m->zeilenIndex = NULL;
    m->spaltenIndex = NULL;
}
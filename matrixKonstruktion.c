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

    //  Blöcke (0-3, 4-7, 8-11, 12-15)
    for (int b = 0; b < 4; b++) {
        int sprung = b * 4;

        for (int i = 0; i < 4; i++) {
            // erste zeile eines blockes setzen
            matrix.zeilenIndex[sprung + i] = zähler;

            // 3 einsen pro zeile
            // sprung versetzt diagonal den block
            //zeile 0 in jedem diagonalblock
            if (i == 0) {
                matrix.spaltenIndex[zähler] = sprung + 0; matrix.wert[zähler] = 1.0; zähler++;
                matrix.spaltenIndex[zähler] = sprung + 1; matrix.wert[zähler] = 1.0; zähler++;
                matrix.spaltenIndex[zähler] = sprung + 3; matrix.wert[zähler] = 1.0; zähler++;
            }
            //zeile 1 in jedem diagonalblock
            else if (i == 1) {
                matrix.spaltenIndex[zähler] = sprung + 0; matrix.wert[zähler] = 1.0; zähler++;
                matrix.spaltenIndex[zähler] = sprung + 1; matrix.wert[zähler] = 1.0; zähler++;
                matrix.spaltenIndex[zähler] = sprung + 2; matrix.wert[zähler] = 1.0; zähler++;
            }
            //zeile 2 in jedem diagonalblock
            else if (i == 2) {
                matrix.spaltenIndex[zähler] = sprung + 1; matrix.wert[zähler] = 1.0; zähler++;
                matrix.spaltenIndex[zähler] = sprung + 2; matrix.wert[zähler] = 1.0; zähler++;
                matrix.spaltenIndex[zähler] = sprung + 3; matrix.wert[zähler] = 1.0; zähler++;
            }
            //zeile 3 in jedem diagonalblock
            else if (i == 3) {
                matrix.spaltenIndex[zähler] = sprung + 0; matrix.wert[zähler] = 1.0; zähler++;
                matrix.spaltenIndex[zähler] = sprung + 2; matrix.wert[zähler] = 1.0; zähler++;
                matrix.spaltenIndex[zähler] = sprung + 3; matrix.wert[zähler] = 1.0; zähler++;
            }
        }

    }

    matrix.zeilenIndex[16] = zähler; // Ende markieren

    return matrix;
}





void print_matrix(SparseMatrix matrix) {
    printf("\n---  Matrix (diagonale 4x4 block) ---\n");
    for (int i = 0; i < 16; i++) {
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

// Matrix als besser lesbare tabelle
void print_matrix_tabelle(SparseMatrix matrix) {
    printf("\n Visuelle Matrix (%dx%d) ---\n", matrix.n, matrix.n);
    printf("     ");
    for(int j = 0; j < matrix.n; j++) printf("%2d  ", j ); // Spaltennummerierung
    printf("\n    -");
    for(int j = 0; j < matrix.n; j++) printf("----");
    printf("\n");

    for (int i = 0; i < matrix.n; i++) {
        printf("%2d | ", i); // Zeilennummer am Rand

        int start = matrix.zeilenIndex[i];
        int ende = matrix.zeilenIndex[i+1];

        //Spalten iteration
        for (int j = 0; j < matrix.n; j++) {
            int eintragExistiert = 0;
            //prüfen, ob im Eintrag in Zeile i und spalte j existiert
            for (int k = start; k < ende; k++) {
                if (matrix.spaltenIndex[k] == j) {
                    eintragExistiert = 1;
                    break;
                }
            }

            if (eintragExistiert) {
                printf(" X  "); // Eine Eins
            } else {
                printf("    "); // Eine Null / Leerstelle
            }
        }
        printf("\n");
    }

}

void loesche_matrix(SparseMatrix *m) {
    if (m->wert != NULL) free(m->wert);
    if (m->zeilenIndex != NULL) free(m->zeilenIndex);
    if (m->spaltenIndex != NULL) free(m->spaltenIndex);
    m->wert = NULL;
    m->zeilenIndex = NULL;
    m->spaltenIndex = NULL;
}
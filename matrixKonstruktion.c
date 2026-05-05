#include <stdio.h>
#include <stdlib.h>
#include "Matrixstruktur.h"


FlexibleSparseMatrix konstruiere_flexible_sparse_matrixmatrix(int d1, int d2, int d3, int d4) {

    FlexibleSparseMatrix m;
    m.d1 = d1; m.d2 = d2; m.d3 = d3; m.d4 = d4;

    // Gesamtzahl der Knoten im 4D-Gitter
    m.knotenAnzahl = m.d1 * m.d2 * m.d3 * m.d4;

    // Jeder Knoten hat genau 8 Verbindungen
    m.nne = m.knotenAnzahl * 8;


    //Speicher reservieren
    // Speicher für die kantenanahl=nne
    m.wert = malloc(m.nne *  sizeof(int));
    m.spalten_indizes = malloc(m.nne *  sizeof(int));
    m.zeilen_zeiger = malloc((m.knotenAnzahl +1) * sizeof(int));


    //Einträge
    int aktueller_eintrag_im_array = 0;

    //Vierte Dimension
    for (int l = 0; l < m.d4; l++) {
        //Dritte Dimension
        for (int k= 0; k < m.d3; k++) {
            //Zweite Dimension
            for (int j= 0; j < m.d2; j++) {
                //Erste Dimension
                for (int i = 0; i < m.d1; i++) {

                    // Linearen Index des aktuellen Knotens (der aktuellen Zeile) berechnen
                    int zeilen_index = i + (j * d1) + (k * d1 * d2) + (l * d1 * d2 * d3);

                    // Zeilenzeiger setzen: Hier beginnen die 8 Nachbarn dieses Knotens
                    m.zeilen_zeiger[zeilen_index] = aktueller_eintrag_im_array;


                    // Hilfs-Array für die Offsets der 8 Nachbarn (Periodische Randbedingungen)
                    // i Position auf der X-Achse (links/rechts)
                    // j Position auf der Y-Achse (oben/unten)
                    // k Position auf der Z-Achse (vorne/hinten "Tiefe")
                    // l Pos in der vierten Dimension

                    int nachbarn[8];
                    nachbarn[0] = ((i + 1) % d1) + (j * d1) + (k * d1 * d2) + (l * d1 * d2 * d3); // Rechts
                    nachbarn[1] = ((i - 1 + d1) % d1) + (j * d1) + (k * d1 * d2) + (l * d1 * d2 * d3); // Links

                    nachbarn[2] = i + (((j + 1) % d2) * d1) + (k * d1 * d2) + (l * d1 * d2 * d3); // Unten
                    nachbarn[3] = i + (((j - 1 + d2) % d2) * d1) + (k * d1 * d2) + (l * d1 * d2 * d3); // Oben

                    nachbarn[4] = i + (j * d1) + (((k + 1) % d3) * d1 * d2) + (l * d1 * d2 * d3); // Vorne
                    nachbarn[5] = i + (j * d1) + (((k - 1 + d3) % d3) * d1 * d2) + (l * d1 * d2 * d3); // Hinten

                    nachbarn[6] = i + (j * d1) + (k * d1 * d2) + (((l + 1) % d4) * d1 * d2 * d3); // Nächster Cube
                    nachbarn[7] = i + (j * d1) + (k * d1 * d2) + (((l - 1 + d4) % d4) * d1 * d2 * d3); // Vorheriger Cube

                    // Die 8 Nachbarn in die CRS-Struktur schreiben
                    for (int n = 0; n < 8; n++) {
                        m.wert[aktueller_eintrag_im_array] = 1; // Gewicht der Kante
                        m.spalten_indizes[aktueller_eintrag_im_array] = nachbarn[n];
                        aktueller_eintrag_im_array++;
                    }
                }
            }
        }

    }
    // Der letzte Zeilenzeiger zeigt auf das Ende der Daten
    m.zeilen_zeiger[m.knotenAnzahl] = m.nne;
    return m;

}


SparseMatrix konstruiere_matrix() {

    SparseMatrix matrix;
    matrix.n = 16;
    matrix.nne = 100;

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

            int zeile = sprung + i;


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

            // NebenDiagonale (nach "rechts" in der Matrix)
            if (zeile + 4 < 16) {
                matrix.spaltenIndex[zähler] = zeile + 4;
                matrix.wert[zähler] = 1.0;
                zähler++;
            }

            // NebenDiagonale (nach "links" in der Matrix bzw unterdiagonale)
            if (zeile - 4 >= 0) {
                matrix.spaltenIndex[zähler] = zeile - 4;
                matrix.wert[zähler] = 1.0;
                zähler++;
            }

            // zweite Nebendiagonale (- 12 also links in)
            if (zeile - 12 >= 0) {
                matrix.spaltenIndex[zähler] = zeile - 12;
                matrix.wert[zähler] = 1.0;
                zähler++;
            }

            //zweote Nebendiagonale rechts (+12)
            if (zeile + 12 < 16) {
                matrix.spaltenIndex[zähler] = zeile + 12;
                matrix.wert[zähler] = 1.0;
                zähler++;
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
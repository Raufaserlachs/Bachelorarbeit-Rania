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
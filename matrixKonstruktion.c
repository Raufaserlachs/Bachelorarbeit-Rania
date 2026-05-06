#include <stdio.h>
#include <stdlib.h>
#include "Matrixstruktur.h"


FlexibleSparseMatrix konstruiere_flexible_matrix(int d1, int d2, int d3, int d4) {

    FlexibleSparseMatrix m;
    m.d1 = d1; m.d2 = d2; m.d3 = d3; m.d4 = d4;

    // Gesamtzahl der Knoten im 4D-Gitter
    m.knotenAnzahl = m.d1 * m.d2 * m.d3 * m.d4;

    // Jeder Knoten hat genau 8 Verbindungen
    m.nne = m.knotenAnzahl * 8;


    //Speicher reservieren
    // Speicher für die kantenanahl=nne
    m.eintraege = malloc(m.nne * sizeof(MatrixEintrag));


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


                    // Hilfs-Array für die 8 Nachbarn
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

                    // Die 8 Nachbarn in das array schreiben
                    for (int n = 0; n < 8; n++) {
                        m.eintraege[aktueller_eintrag_im_array].i = zeilen_index; // ZEILE
                        m.eintraege[aktueller_eintrag_im_array].j = nachbarn[n];  // SPALTE
                        m.eintraege[aktueller_eintrag_im_array].wert = 1;         // WERT
                        aktueller_eintrag_im_array++;
                    }
                }
            }
        }

    }

    return m;

}

void print_flexible_matrix_tabelle(FlexibleSparseMatrix m) {
    // Knotenanahl für x y matrix
    int n = m.knotenAnzahl;

    //  Nur drucken, wenn die Matrix nicht zu riesig ist
    if (n > 100) {
        printf("\n Matrix ist mit zu groß für die Tabellenansicht! ---\n");
        return;
    }



    // Spaltennummerierung oben
    printf("     ");
    for(int j = 0; j < n; j++) printf("%2d ", j);
    printf("\n    -");
    for(int j = 0; j < n; j++) printf("---");
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf("%2d | ", i); // Zeilennummer am Rand

        for (int j = 0; j < n; j++) {
            int hatNachbar = 0;

            // suche nur nach Einträgen, die zur aktuellen Zeile i gehören
            for (int k = 0; k < m.nne; k++) {

                // wenn Eintrag für Zeile i und Spalte j gefunden
                if (m.eintraege[k].i == i && m.eintraege[k].j == j) {
                    hatNachbar = 1;
                    break;
                }
                // wenn nächsten Zeileneinträge erreicht,  abbrechen
                if (m.eintraege[k].i > i) break;
            }

            if (hatNachbar) printf(" X "); else printf(" . ");
        }
        printf("\n");
    }
        printf("------------------------------------------\n");

}









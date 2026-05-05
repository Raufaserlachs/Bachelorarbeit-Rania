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

void print_flexible_matrix_tabelle(FlexibleSparseMatrix m) {
    // Knotenanahl für x y matrix
    int n = m.knotenAnzahl;

    //  Nur drucken, wenn die Matrix nicht zu riesig ist!
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

        int start = m.zeilen_zeiger[i];
        int ende = m.zeilen_zeiger[i+1];

        //  jede mögliche Spalte j durchgehen
        for (int j = 0; j < n; j++) {
            int hatNachbar = 0;

            // schauen nach, ob Spalte j in Zeile i vorkommt
            for (int k = start; k < ende; k++) {
                if (m.spalten_indizes[k] == j) {
                    hatNachbar = 1;
                    break;
                }
            }

            if (hatNachbar) {
                printf(" X "); // Hier ist eine Kante/Eintrag
            } else {
                printf(" . "); // Keine Keine Kante/eintrag
            }
        }
        printf("\n");
    }
        printf("------------------------------------------\n");

}









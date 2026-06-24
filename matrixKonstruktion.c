#include <stdio.h>
#include <stdlib.h>
#include <time.h>   // Für Zufallszahlen
#include <math.h>
#include "Matrixstruktur.h"

FlexibleSparseMatrix konstruiere_flexible_matrix(int d1, int d2, int d3, int d4, int B) {
    srand(time(NULL));
    FlexibleSparseMatrix m;
    m.d1 = d1; m.d2 = d2; m.d3 = d3; m.d4 = d4;
    m.knotenAnzahl = d1 * d2 * d3 * d4;
    m.B = B;

    // Speicherbedarf: 9 Verbindungen (8 Nachbarn + Diagonale) * (B * B) Einträge
    m.nne = m.knotenAnzahl * 9 * (B * B);
    m.eintraege = malloc(m.nne * sizeof(MatrixEintrag));

    // umrechnung der indizijes
    int *mapping = malloc(m.knotenAnzahl * sizeof(int));
    int w_idx = 0;
    int s_idx = (m.knotenAnzahl / 2);
    for (int i = 0; i < m.knotenAnzahl; i++) {
        int tmp = i;
        int x = tmp % d1; tmp /= d1;
        int y = tmp % d2; tmp /= d2;
        int z = tmp % d3; tmp /= d3;
        int w = tmp;
        if ((x + y + z + w) % 2 == 0) mapping[i] = w_idx++;
        else mapping[i] = s_idx++;
    }

    //  Matrix füllen
    int eintrag_idx = 0;
    int dx[] = {1, -1, 0, 0, 0, 0, 0, 0};
    int dy[] = {0, 0, 1, -1, 0, 0, 0, 0};
    int dz[] = {0, 0, 0, 0, 1, -1, 0, 0};
    int dw[] = {0, 0, 0, 0, 0, 0, 1, -1};

    for (int i = 0; i < m.knotenAnzahl; i++) {
        int block_start_i = mapping[i] * B;
        int x = i % d1; int tmp = i / d1;
        int y = tmp % d2; tmp /= d2;
        int z = tmp % d3; tmp /= d3;
        int w = tmp;

        // A) Diagonale (Block i zu i)
        for (int r = 0; r < B; r++) {
            for (int c = 0; c < B; c++) {
                m.eintraege[eintrag_idx].i = block_start_i + r;
                m.eintraege[eintrag_idx].j = block_start_i + c;
                m.eintraege[eintrag_idx].wert = (r == c) ? 8.0 + (double)rand()/RAND_MAX : (double)rand()/RAND_MAX;
                eintrag_idx++;
            }
        }

        // B) 8 Nachbarn (Block i zu nachbar)
        for (int n = 0; n < 8; n++) {
            int nx = (x + dx[n] + d1) % d1;
            int ny = (y + dy[n] + d2) % d2;
            int nz = (z + dz[n] + d3) % d3;
            int nw = (w + dw[n] + d4) % d4;
            int nachbar_idx = nx + (ny * d1) + (nz * d1 * d2) + (nw * d1 * d2 * d3);
            int block_start_j = mapping[nachbar_idx] * B;

            for (int r = 0; r < B; r++) {
                for (int c = 0; c < B; c++) {
                    m.eintraege[eintrag_idx].i = block_start_i + r;
                    m.eintraege[eintrag_idx].j = block_start_j + c;
                    m.eintraege[eintrag_idx].wert = (double)rand() / RAND_MAX;
                    eintrag_idx++;
                }
            }
        }
    }
    free(mapping);
    return m;
}

void print_flexible_matrix_tabelle(FlexibleSparseMatrix m) {
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









#include <stdio.h>
#include <stdlib.h>
#include "matrixstruktur.h"
//1, 1, 0, 1, 1, 0, 0, 0, 0, 0,  0,  0,  1,  0,  0,  0,
//1, 1, 1, 0, 0, 1, 0, 0, 0, 0,  0,  0,  0,  1,  0,  0,
//0, 1, 1, 1, 0, 0, 1, 0, 0, 0,  0,  0,  0,  0,  1,  0,
//1, 0, 1, 1, 0, 0, 0, 1, 0, 0,  0,  0,  0,  0,  0,  1,
//1, 0, 0, 0, 1, 1, 0, 1, 1, 0,  0,  0,  0,  0,  0,  0,
//0, 1, 0, 0, 1, 1, 1, 0, 0, 1,  0,  0,  0,  0,  0,  0,
//0, 0, 1, 0, 0, 1, 1, 1, 0, 0,  1,  0,  0,  0,  0,  0,
//0, 0, 0, 1, 1, 0, 1, 1, 0, 0,  0,  1,  0,  0,  0,  0,
//0, 0, 0, 0, 1, 0, 0, 0, 1, 1,  0,  1,  1,  0,  0,  0,
//0, 0, 0, 0, 0, 1, 0, 0, 1, 1,  1,  0,  0,  1,  0,  0,
//0, 0, 0, 0, 0, 0, 1, 0, 0, 1,  1,  1,  0,  0,  1,  0,
//0, 0, 0, 0, 0, 0, 0, 1, 1, 0,  1,  1,  0,  0,  0,  1,
//1, 0, 0, 0, 0, 0, 0, 0, 1, 0,  0,  0,  1,  1,  0,  1,
//0, 1, 0, 0, 0, 0, 0, 0, 0, 1,  0,  0,  1,  1,  1,  0,
//0, 0, 1, 0, 0, 0, 0, 0, 0, 0,  1,  0,  0,  1,  1,  1,
//0, 0, 0, 1, 0, 0, 0, 0, 0, 0,  0,  1,  1,  0,  1,  1,

// effiziente Lösung linearer Gleichungssysteme
int main(void) {
    // Matrix konstruieren
    FlexibleSparseMatrix meinesparseMatrix = konstruiere_flexible_matrix(2, 2, 2, 2);

    // Matrix Konvertieren
    DichteMatrix meineMatrix = konvertiere_zu_dicht(meinesparseMatrix);

    // Vorher-Zustand zeigen
    printf("VOR DEM GAUSS:");
    drucke_dichte_matrix(meineMatrix);

    // Testlauf starten
    testlauf(meineMatrix);

    // Speicher aufräumen
    free(meinesparseMatrix.eintraege);
    freigabe_dichte_matrix(meineMatrix);

    printf("Speicher erfolgreich bereinigt.\n");

    return 0;
}
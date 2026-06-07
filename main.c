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

int main(void) {
    // 1. Matrix konstruieren (die Datenquelle)
    FlexibleSparseMatrix meinesparseMatrix = konstruiere_flexible_matrix(2, 2, 2, 2);

    // 2. Matrix Konvertieren (in eine DichteMatrix für den Solver)
    DichteMatrix meineMatrix = konvertiere_zu_dicht(meinesparseMatrix);

    // 3. Vorher-Zustand zeigen
    printf("VOR DEM GAUSS:");
    drucke_dichte_matrix(meineMatrix);

    // 4. Testlauf starten (Hier passiert alles: Kopie, Gauß, Rücksubstitution, Residuum)
    // Deine testlauf-Funktion erledigt jetzt das "schwere Heben"
    testlauf(meineMatrix);

    // 5. Speicher aufräumen
    // Wichtig: Erst die Einträge der Sparse, dann die DichteMatrix freigeben
    free(meinesparseMatrix.eintraege);
    freigabe_dichte_matrix(meineMatrix);

    printf("Speicher erfolgreich bereinigt.\n");

    return 0;
}
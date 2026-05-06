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


    // Matrix konstruieren
    FlexibleSparseMatrix meineMatrix = konstruiere_flexible_matrix(4, 4, 1, 1);

    // Tabellen-Ansicht für das Muster
    print_flexible_matrix_tabelle(meineMatrix);

    // Speicher freigeben
    free(meineMatrix.eintraege);


    //  Gauß-Algorithmus ..später
    // solve_gauss oder so


    printf("Speicher erfolgreich bereinigt.\n");

    return 0;

}
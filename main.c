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
    FlexibleSparseMatrix meineMatrix = konstruiere_flexible_matrix(3, 3, 3, 3);

    // Tabellen-Ansicht für das Muster
    print_flexible_matrix_tabelle(meineMatrix);

    // Speicher freigeben
    free(meineMatrix.wert);
    free(meineMatrix.spalten_indizes);
    free(meineMatrix.zeilen_zeiger);

    //  Gauß-Algorithmus ..später
    // solve_gauss oder so


    printf("Speicher erfolgreich bereinigt.\n");

    return 0;

}
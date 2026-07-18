#include <stdio.h>
#include <stdlib.h>
#include "matrixstruktur.h"




// effiziente Lösung linearer Gleichungssysteme
int main(void) {


    // Matrix konstruieren
    FlexibleSparseMatrix meinesparseMatrix = konstruiere_flexible_matrix(2, 2, 2, 2,4);

    // Matrix Konvertieren
    CSRMatrix meineMatrix = konvertiere_zu_optimierten_csr(meinesparseMatrix);

    // Vorher-Zustand zeigen
    printf("VOR DEM GAUSS:");

    drucke_optimierte_csr(meineMatrix);
    // drucke_csr_matrix(meineMatrix);



    // Testlauf starten
    testlauf_csr(meineMatrix);
    //testlauf(meineMatrix);


    drucke_csr_nach_gauss(meineMatrix);

    //Speicher aufräumen
    free(meinesparseMatrix.eintraege);
    freigabe_csr_matrix(meineMatrix);



    printf("Speicher erfolgreich bereinigt.\n");






    return 0;
}
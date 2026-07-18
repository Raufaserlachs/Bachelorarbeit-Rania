#include <stdio.h>
#include <stdlib.h>
#include "matrixstruktur.h"




// effiziente Lösung linearer Gleichungssysteme
int main(void) {


    // Matrix konstruieren
    FlexibleSparseMatrix meinesparseMatrix = konstruiere_flexible_matrix(2, 2, 2, 2,4);

    // Matrix Konvertieren
    BCSRMatrix meineMatrix = konvertiere_zu_bcsr(meinesparseMatrix);

    // Vorher-Zustand zeigen
    printf("VOR DEM GAUSS:");
    drucke_bcsr_vor_gauss(meineMatrix);
    //drucke_optimierte_csr(meineMatrix);
    // drucke_csr_matrix(meineMatrix);

    bringe_in_zeilenstufenform_bcsr(meineMatrix, meineMatrix.val);

    // Testlauf starten
    //testlauf_csr(meineMatrix);
    //testlauf(meineMatrix);

    drucke_bcsr_nach_gauss(meineMatrix);
    //drucke_csr_nach_gauss(meineMatrix);

    //Speicher aufräumen
    free(meinesparseMatrix.eintraege);
    //freigabe_csr_matrix(meineMatrix);
    freigeben_bcsr_matrix(&meineMatrix);



    printf("Speicher erfolgreich bereinigt.\n");






    return 0;
}
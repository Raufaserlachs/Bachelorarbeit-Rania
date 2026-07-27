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

    drucke_csr_matrix(meineMatrix);
    // drucke_csr_matrix(meineMatrix);

    int gesamt_dim = meineMatrix.N;
    double *b = malloc(gesamt_dim * sizeof(double));
    for (int i = 0; i < gesamt_dim; i++) b[i] = 1.0;



    bringe_in_zeilenstufenform_csr(meineMatrix, b);


    drucke_csr_matrix(meineMatrix);

    double *x = erstelle_loesungsvektor(meineMatrix.N);

    loese_rueckwaertssubstitution_csr(meineMatrix , b, x);

    drucke_vektor_x(x , meineMatrix.N );
    // Testlauf starten
    //testlauf(meineMatrix);

    //Speicher aufräumen
    free(meinesparseMatrix.eintraege);
    freigabe_csr_matrix(meineMatrix);
    free(b);
    free(x);
    printf("Speicher erfolgreich bereinigt.\n");






    return 0;
}
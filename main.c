#include <stdio.h>
#include <stdlib.h>
#include "matrixstruktur.h"




// effiziente Lösung linearer Gleichungssysteme
int main(void) {



    // Basis-Matrix konstruieren (FlexibleSparseMatrix als Ausgangsbasis)
    FlexibleSparseMatrix meinesparseMatrix = konstruiere_flexible_matrix(4, 4, 4, 4, 4);
    int gesamt_dim = meinesparseMatrix.knotenAnzahl * meinesparseMatrix.B;

    //double *x_dicht = malloc(gesamt_dim * sizeof(double));
    double *x_csr = malloc(gesamt_dim * sizeof(double));



    // printf("========================================\n");
    // printf(" 1. TESTLAUF: KLASSISCHER DICHTER SOLVER\n");
    // printf("========================================\n");
    //
    // // Für den dichten Solver wandeln wir die FlexibleMatrix einmalig in eine DichteMatrix um
    // DichteMatrix dichte_original = konvertiere_zu_dicht(meinesparseMatrix);
    //
    // // Testlauf für den dichten Solver starten (wie gewohnt mit eigenem b und Validierung)
    // testlauf(dichte_original, x_dicht);
    //
    // // Aufräumen der dichten Testmatrix
    // freigabe_dichte_matrix(dichte_original);


    printf("\n========================================\n");
    printf(" 2. TESTLAUF: OPTIMIERTER CSR-SOLVER\n");
    printf("========================================\n");

    messe_performance(meinesparseMatrix);
    // Matrix in das optimierte CSR-Format konvertieren (mit symbolischer Faktorisierung)
    // CSRMatrix meineMatrix = konvertiere_zu_optimierten_csr(meinesparseMatrix);

    // Vorher-Zustand zeigen
    //printf("VOR DEM GAUSS (CSR):\n");
    ////drucke_csr_matrix(meineMatrix);

    // Testlauf für das CSR-Format starten (ruft deinen neuen Testlauf auf,
    // der b erzeugt, den Gauß ausführt, rückwärts substituiert und das Residuum prüft!)
    //testlauf_csr(meineMatrix, x_csr);



    ////vergleiche_loesungen( x_dicht, x_csr, gesamt_dim);


    // Speicher aufräumen
    free(meinesparseMatrix.eintraege);
    //freigabe_csr_matrix(meineMatrix);

    printf("Speicher erfolgreich bereinigt.\n");
    return 0;
}
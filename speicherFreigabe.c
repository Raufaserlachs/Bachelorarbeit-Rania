//
// Created by Nia on 16.07.26.
//

#include <stdlib.h>
#include "matrixstruktur.h"


void freigabe_csr_matrix(CSRMatrix A) {
    free(A.val);
    free(A.ci);
    free(A.rst);
}


void freigabe_dichte_matrix(DichteMatrix A) {
    //Erst die einzelnen Zeilen freigeben
    for (int i = 0; i < A.N; i++) {
        free(A.daten[i]);
    }
    // Dann den "Haupt-Zeiger" für die Zeilen-Pointer freigeben
    free(A.daten);
}

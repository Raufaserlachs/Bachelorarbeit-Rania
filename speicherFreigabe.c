//
// Created by Nia on 16.07.26.
//

#include <stdlib.h>
#include "Matrixstruktur.h"


void freigabe_csr_matrix(CSRMatrix A) {
    free(A.val);
    free(A.ci);
    free(A.rst);
}


//Speicherpflege
void freigabe_dichte_matrix(DichteMatrix A) {
    // 1. Erst die einzelnen Zeilen freigeben
    for (int i = 0; i < A.N; i++) {
        free(A.daten[i]);
    }
    // 2. Dann den "Haupt-Zeiger" für die Zeilen-Pointer freigeben
    free(A.daten);
}

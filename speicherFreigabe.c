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


//Speicherpflege
void freigabe_dichte_matrix(DichteMatrix A) {
    // 1. Erst die einzelnen Zeilen freigeben
    for (int i = 0; i < A.N; i++) {
        free(A.daten[i]);
    }
    // 2. Dann den "Haupt-Zeiger" für die Zeilen-Pointer freigeben
    free(A.daten);
}

/**
 * Gibt den Speicher einer BCSRMatrix frei.
 */
void freigeben_bcsr_matrix(BCSRMatrix *A) {
    if (A == NULL) return;

    if (A->row_ptr != NULL) {
        free(A->row_ptr);
        A->row_ptr = NULL;
    }
    if (A->col_idx != NULL) {
        free(A->col_idx);
        A->col_idx = NULL;
    }
    if (A->val != NULL) {
        free(A->val);
        A->val = NULL;
    }

    A->N = 0;
    A->B = 0;
}
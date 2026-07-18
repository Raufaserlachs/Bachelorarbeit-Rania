//
// Created by Nia on 17.07.26.
//



#include "matrixstruktur.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * Invertiert einen BxB Block in-place mit Gauß-Jordan Elimination.
 * mat: Zeiger auf den B*B Block (Zeilen-Hauptordnung)
 * B: Blockgröße
 */
void invert_block(double *mat, int B) {
    // Erstelle temporäre augmentierte Matrix [mat | I]
    double *tmp = calloc(B * 2 * B, sizeof(double));
    for (int i = 0; i < B; i++) {
        for (int j = 0; j < B; j++) {
            tmp[i * (2 * B) + j] = mat[i * B + j];
        }
        tmp[i * (2 * B) + (i + B)] = 1.0; // Einheitsmatrix auf der rechten Seite
    }

    // Gauß-Jordan Elimination
    for (int i = 0; i < B; i++) {
        // Pivot-Suche (vereinfacht, für kleine Blöcke meist okay)
        double pivot = tmp[i * (2 * B) + i];

        // Zeile normalisieren
        for (int j = 0; j < 2 * B; j++) {
            tmp[i * (2 * B) + j] /= pivot;
        }

        // Andere Zeilen eliminieren
        for (int k = 0; k < B; k++) {
            if (k != i) {
                double factor = tmp[k * (2 * B) + i];
                for (int j = 0; j < 2 * B; j++) {
                    tmp[k * (2 * B) + j] -= factor * tmp[i * (2 * B) + j];
                }
            }
        }
    }

    // Kopiere Ergebnis zurück in das Original-Feld
    for (int i = 0; i < B; i++) {
        for (int j = 0; j < B; j++) {
            mat[i * B + j] = tmp[i * (2 * B) + (j + B)];
        }
    }

    free(tmp);
}


// Multipliziert zwei Blöcke: res = a * b
void block_mul(double *a, double *b, double *res, int B) {
    for (int i = 0; i < B; i++) {
        for (int j = 0; j < B; j++) {
            res[i * B + j] = 0;
            for (int k = 0; k < B; k++)
                res[i * B + j] += a[i * B + k] * b[k * B + j];
        }
    }
}

// Subtrahiert Block-Produkt: target -= factor * block
void bcsr_sub_mul(double *target, double *factor, double *block, int B) {
    for (int i = 0; i < B; i++) {
        for (int j = 0; j < B; j++) {
            double sum = 0;
            for (int k = 0; k < B; k++)
                sum += factor[i * B + k] * block[k * B + j];
            target[i * B + j] -= sum;
        }
    }
}

// Wert in CSR setzen (Wichtig für Fill-in!)
void set_value_csr(CSRMatrix A, int row, int col, double val) {
    if (col < row) { int tmp = row; row = col; col = tmp; }
    for (int k = A.rst[row]; k < A.rst[row + 1]; k++) {
        if (A.ci[k] == col) {
            A.val[k] = val;
            return;
        }
    }
}

void bringe_in_zeilenstufenform_bcsr(BCSRMatrix A, double *b) {
    int B = A.B;
    for (int i = 0; i < A.N; i++) {
        // 1. Pivot-Block finden (A[i][i]) und invertieren
        double *pivot_block = NULL;
        for (int p = A.row_ptr[i]; p < A.row_ptr[i + 1]; p++) {
            if (A.col_idx[p] == i) { pivot_block = &A.val[p * B * B]; break; }
        }
        invert_block(pivot_block, B); // Invertiert den Block in-place

        // 2. Elimination für alle Zeilen unterhalb
        for (int j = i + 1; j < A.N; j++) {
            double *ziel_block = NULL;
            for (int p = A.row_ptr[j]; p < A.row_ptr[j + 1]; p++) {
                if (A.col_idx[p] == i) { ziel_block = &A.val[p * B * B]; break; }
            }
            if (!ziel_block) continue;

            // Faktor berechnen: Factor = Ziel_Block * Inverse(Pivot_Block)
            double factor[B * B];
            block_mul(ziel_block, pivot_block, factor, B);

            // Update rechte Seite b: b[j] = b[j] - Faktor * b[i]
            for(int r=0; r<B; r++) {
                double sum = 0;
                for(int k=0; k<B; k++) sum += factor[r*B + k] * b[i*B + k];
                b[j*B + r] -= sum;
            }

            // Update Rest der Zeile
            for (int k = i + 1; k < A.N; k++) {
                double *val_i = NULL, *val_j = NULL;
                for (int p = A.row_ptr[i]; p < A.row_ptr[i + 1]; p++) if (A.col_idx[p] == k) { val_i = &A.val[p * B * B]; break; }
                for (int p = A.row_ptr[j]; p < A.row_ptr[j + 1]; p++) if (A.col_idx[p] == k) { val_j = &A.val[p * B * B]; break; }

                if (val_i && val_j) bcsr_sub_mul(val_j, factor, val_i, B);
            }
        }
    }
}


// void loese_mit_ruecksubstitution_csr(CSRMatrix A, double b[], double x[]) {
//     int N = A.N;
//
//     for (int i = N - 1; i >= 0; i--) {
//         double summe = 0.0;
//
//         // Summiere A[i][j] * x[j]
//         // In CSR: Nur über die gespeicherten Spalten der Zeile i laufen
//         for (int k = A.rst[i]; k < A.rst[i + 1]; k++) {
//             int j = A.ci[k];
//             if (j > i) {
//                 summe += A.val[k] * x[j];
//             }
//         }
//
//         double diag = get_value_csr_raw(A, i, i);
//         x[i] = (b[i] - summe) / diag;
//     }
// }
//
//










//gen mat ( d1 d2 d3 d4 und k) für bblocke
//mit RB formAT
//DÜNNBESETZER GAUSS ..... wo nicht null elemente und dann handlung nur wo nicht null auch in zukunft

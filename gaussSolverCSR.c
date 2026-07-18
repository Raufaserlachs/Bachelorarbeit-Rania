//
// Created by Nia on 17.07.26.
//



#include "matrixstruktur.h"



// Wert aus CSR holen
double get_value_csr(CSRMatrix A, int row, int col) {
    if (col < row) { int tmp = row; row = col; col = tmp; } // Symmetrie-Spiegelung
    for (int k = A.rst[row]; k < A.rst[row + 1]; k++) {
        if (A.ci[k] == col) return A.val[k];
    }
    return 0.0;
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

//Änderung: Transformationsvektor
//Funktion für reine ZSF also Gauss vorwärts
void bringe_in_zeilenstufenform_csr(CSRMatrix A, double b[]) {
    int N = A.N;

    for (int i = 0; i < N - 1; i++) {
        // Wir suchen Zeile i als Pivot-Zeile
        // In CSR: Zeile i beginnt bei A.rst[i]
        for (int j = i + 1; j < N; j++) {

            // Finde Wert A[j][i] (benötigt Spiegelung, da nur oberes Dreieck gespeichert)
            // Wenn A symmetrisch ist: A[j][i] ist das gleiche wie A[i][j]
            double pivot_element = get_value_csr(A, i, i); // Diagonale
            double ziel_element = get_value_csr(A, j, i);   // Unteres Dreieck gespiegelt

            if (ziel_element == 0.0) continue; // Überspringe, wenn schon 0

            double faktor = ziel_element / pivot_element;
            b[j] -= b[i] * faktor;

            // Subtrahiere Zeile i von Zeile j
            // Hier musst du beide Zeilen als CSR-Strukturen "abfahren"
            for (int k = i + 1; k < N; k++) {
                double val_i = get_value_csr(A, i, k);
                if (val_i != 0.0) {
                    double val_j = get_value_csr(A, j, k); // Hier findet Fill-in statt!
                    set_value_csr(A, j, k, val_j - val_i * faktor);
                }
            }
        }
    }
}



void loese_mit_ruecksubstitution_csr(CSRMatrix A, double b[], double x[]) {
    int N = A.N;

    for (int i = N - 1; i >= 0; i--) {
        double summe = 0.0;

        // Summiere A[i][j] * x[j]
        // In CSR: Nur über die gespeicherten Spalten der Zeile i laufen
        for (int k = A.rst[i]; k < A.rst[i + 1]; k++) {
            int j = A.ci[k];
            if (j > i) {
                summe += A.val[k] * x[j];
            }
        }

        double diag = get_value_csr(A, i, i);
        x[i] = (b[i] - summe) / diag;
    }
}












//gen mat ( d1 d2 d3 d4 und k) für bblocke
//mit RB formAT
//DÜNNBESETZER GAUSS ..... wo nicht null elemente und dann handlung nur wo nicht null auch in zukunft

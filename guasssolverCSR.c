//
// Created by Nia on 21.07.26.
//

#include "matrixstruktur.h"

/**
 *  Holt einen Wert aus der CSR-Matrix.
 * Gibt 0.0 zurück, wenn der Spaltenindex in dieser Zeile nicht existiert.
 */
double get_value_csr(CSRMatrix A, int row, int col) {
    for (int k = A.rst[row]; k < A.rst[row + 1]; k++) {
        if (A.ci[k] == col) {
            return A.val[k];
        }
    }
    return 0.0;
}

/**
 * Setzt oder aktualisiert einen Wert in der CSR-Matrix.
 * (Voraussetzung: Der Platz existiert im CSR-Format, was durch die vorherige Allokierung sichergestellt ist).
 */
void set_value_csr(CSRMatrix A, int row, int col, double val) {
    for (int k = A.rst[row]; k < A.rst[row + 1]; k++) {
        if (A.ci[k] == col) {
            A.val[k] = val;
            return;
        }
    }
}

/**
 *  Pivot-Prüfung und Schutz gegen Division durch Null.
 */
int ist_gueltiges_pivot(double pivot_wert) {
    return (pivot_wert != 0.0);
}

/**
 *  Anpassung der rechten Seite (Vektor b).
 */
void aktualisiere_rechte_seite(double b[], int i, int j, double faktor) {
    b[j] -= b[i] * faktor;
}

/**
 * Zeilen-Update (Right-Looking Subtraktion der Pivot-Zeile von der Zielzeile).
 */
void fuehre_right_looking_update(CSRMatrix A, int i, int j, double faktor) {

    for (int k_idx = A.rst[i]; k_idx < A.rst[i + 1]; k_idx++) {
        int col = A.ci[k_idx];

        // Nur Spalten ab der Diagonalen / Pivot-Spalte aktualisieren
        if (col >= i) {
            double val_i = A.val[k_idx];
            double val_j_aktuell = get_value_csr(A, j, col);

            double neuer_wert = val_j_aktuell - (val_i * faktor);

            // Berechneten Wert direkt im vorallokierten Speicher speichern
            set_value_csr(A, j, col, neuer_wert);
        }
    }
    // Eliminations-Element unter dem Pivot explizit auf 0.0 setzen
    set_value_csr(A, j, i, 0.0);
}



/**
 * Bringt die CSRMatrix via Right-Looking Gauß-Elimination (kij-Variante)
 * in Zeilenstufenform und aktualisiert dabei den Vektor b.
 */
void bringe_in_zeilenstufenform_csr(CSRMatrix A, double b[]) {
    int N = A.N;

    // i läuft über die Pivot-Zeile
    for (int i = 0; i < N - 1; i++) {
        double pivot_wert = get_value_csr(A, i, i);

        // Schutz gegen Division durch 0
        if (!ist_gueltiges_pivot(pivot_wert)) continue;

        // j läuft über alle Zeilen unterhalb der Pivot-Zeile
        for (int j = i + 1; j < N; j++) {
            double ziel_wert = get_value_csr(A, j, i);

            // Wenn unter dem Pivot bereits eine 0 steht, Zeile überspringen
            if (ziel_wert == 0.0) continue;

            // Faktor berechnen
            double faktor = ziel_wert / pivot_wert;

            // Rechte Seite b anpassen
            aktualisiere_rechte_seite(b, i, j, faktor);

            // Right-Looking Update auf die Matrix-Zeile anwenden
            fuehre_right_looking_update(A, i, j, faktor);
        }
    }
}





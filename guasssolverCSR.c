//
// Created by Nia on 21.07.26.
//

#include <stdlib.h>
#include "matrixstruktur.h"


long long op_count_zsf = 0;
long long op_count_ruecksub = 0;

/**
 * Holt einen Wert aus der CSR-Matrix.
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
 * (Voraussetzung: Der Platz existiert im CSR-Format dank der symbolischen Faktorisierung).
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
 * Teilaufgabe: Pivot-Prüfung und Schutz gegen Division durch Null.
 */
int ist_gueltiges_pivot(double pivot_wert) {
    return (pivot_wert != 0.0);
}

/**
 * Teilaufgabe: Anpassung der rechten Seite (Vektor b).
 */
void aktualisiere_rechte_seite(double b[], int i, int j, double faktor) {
    b[j] -= b[i] * faktor;
    op_count_zsf += 2; // 1 Multiplikation + 1 Subtraktion = 2 FLOPs

}

/**
 * Aufgabe: Zeilen-Update (Right-Looking Subtraktion der Pivot-Zeile von der Zielzeile).
 */
void fuehre_right_looking_update(CSRMatrix A, int i, int j, double faktor) {
    for (int k_idx = A.rst[i]; k_idx < A.rst[i + 1]; k_idx++) {
        int col = A.ci[k_idx];

        // Nur Spalten ab der Diagonalen / Pivot-Spalte aktualisieren
        if (col >= i) {
            double val_i = A.val[k_idx];
            double val_j_aktuell = get_value_csr(A, j, col);

            double neuer_wert = val_j_aktuell - (val_i * faktor);
            op_count_zsf += 2; // 1 Multiplikation + 1 Subtraktion = 2 FLOPs


            // Berechneten Wert direkt im vorallokierten Speicher speichern
            set_value_csr(A, j, col, neuer_wert);
        }
    }
    // Eliminations-Element unter dem Pivot explizit auf 0.0 setzen
    set_value_csr(A, j, i, 0.0);
}

/**
 * Hauptfunktion: Bringt die CSR Matrix via Right-Looking Gauß-Elimination (kij-Variante)
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
            op_count_zsf += 1;


            // Rechte Seite b anpassen
            aktualisiere_rechte_seite(b, i, j, faktor);

            // Right-Looking Update auf die Matrix-Zeile anwenden
            fuehre_right_looking_update(A, i, j, faktor);
        }
    }
}



/**
 * Erstellt und allokiert dynamisch den Lösungsvektor x der Größe N.
 */
double*erstelle_loesungsvektor(int N) {
    double *x = calloc(N, sizeof(double));
    return x;
}





/**
 * Löst das Gleichungssystem A * x = b nach der Zeilenstufenform
 * mittels Rückwärtssubstitution (Back Substitution) für das CSR-Format.
 * * Ergebnis wird direkt in den Vektor x geschrieben.
 */
void loese_rueckwaertssubstitution_csr(CSRMatrix A, double b[], double x[]) {
    int N = A.N;

    // Starte von der allerletzten Zeile und arbeite dich nach oben vor
    for (int i = N - 1; i >= 0; i--) {
        double summe = 0.0;

        // Summiere alle bereits bekannten Werte x[j] multipliziert mit dem Matrix-Koeffizienten
        // Wir nutzen deine get_value_csr Funktion, die dank Symmetrie/CSR sicher zugreift.
        for (int k = A.rst[i]; k < A.rst[i + 1]; k++) {
            int col = A.ci[k];

            // Wir brauchen nur die Spalten rechts von der Diagonalen (col > i)
            if (col > i) {
                summe += A.val[k] * x[col];
                op_count_ruecksub += 2; // 1 Multiplikation + 1 Addition = 2 FLOPs

            }
        }

        // Diagonalelement holen (das ist der Koeffizient für unser aktuelles x[i])
        double diagonal_wert = get_value_csr(A, i, i);

        // Schutz vor Division durch 0 (falls die Matrix singulär ist)
        if (diagonal_wert == 0.0) {
            // Fehlerbehandlung: z.B. x[i] = 0.0 setzen oder abbrechen
            x[i] = 0.0;
            continue;
        }

        // Eigentliche Formel: x[i] = ( b[i] - Summe ) / A[i][i]
        x[i] = (b[i] - summe) / diagonal_wert;
        op_count_ruecksub += 2; // 1 Subtraktion + 1 Division = 2 FLOPs

    }
}

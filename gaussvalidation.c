//
// Created by Nia on 24.05.26.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>   // Für Zufallszahlen
#include <math.h>
#include "matrixstruktur.h"





// Hilfsfunktion: Berechnet b = A * x (MVP)
void mmvp_dichte(DichteMatrix A, double *x, double *b) {
    for (int i = 0; i < A.N; i++) {
        b[i] = 0.0;
        for (int j = 0; j < A.N; j++) {
            b[i] += A.daten[i][j] * x[j]; // Skalarprodukt: Matrixwert * Vektorwert aufsummieren
        }
    }
}



//Input: Matrix A
//Alg: erzeuge A mal x = b und speichere den Vektor X und b
//Output: vektor X Hardware [1,1,1...]
void gaussTester ( DichteMatrix A ) {
        int N = A.N;
        double zielvektor_x[N];
        double b_out[N];

        // 1. x_target mit Einsen füllen
        for (int i = 0; i < N; i++) {
            zielvektor_x[i] = 1.0;
        }

        // 2. b erzeugen durch b = A * x_target
        mmvp_dichte(A, zielvektor_x, b_out);

        printf("[Validator] Test-Szenario erstellt: b wurde generiert (A * [1,1,...]).\n");


}


// Hilfsfunktion: Normierung
double berechne_norm(double *r, int N) {
    double summe = 0.0;
    for (int i = 0; i < N; i++) {
        summe += r[i] * r[i];
    }
    return sqrt(summe);
}



//  daxpy und nrm
void berechne_residuum(DichteMatrix A_original, double *x_berechnet, double *b_original) {

    int N = A_original.N;
    double r[N]; // Hier speichern wir b - A*x

    // A * x_berechnet ausrechnen (MMVP)
    //  Das Ergebnis von b_orig abziehen (DAXPY-Logik: r = b - A*x)
    for (int i = 0; i < N; i++) {
        double Ax_i = 0.0;
        for (int j = 0; j < N; j++) {
            Ax_i += A_original.daten[i][j] * x_berechnet[j];
        }
        r[i] = b_original[i] - Ax_i;
    }

    // Fehlergröße ausgeben (NRM)
    double fehler = berechne_norm(r, N);
    printf("Das Residuum (Fehler) ist: %e\n", fehler);



}



void testlauf(DichteMatrix A_original) {
    int N = A_original.N;


    double x_exakt[N];       // Was wir als "Lösung" annehmen
    double b_original[N];    // Das b, das zu A * x_exakt passt
    double x_berechnet[N];   // Das Ergebnis, das der Gauß ausspuckt

    // "Wunsch-Lösung" auf lauter Einsen setzen
    for(int i = 0; i < N; i++) x_exakt[i] = 1.0;

    // Erzeuge das b_original (A * x_exakt)
    mmvp_dichte(A_original, x_exakt, b_original);

    // Kopiere A und b, damit der Solver das Original nicht zerstört
    DichteMatrix A_work = kopiere_matrix(A_original);
    double b_work[N];
    for(int i = 0; i < N; i++) b_work[i] = b_original[i];

    // 4. Den eigentlichen Löser rufen
    bringe_in_zeilenstufenform(A_work, b_work);
    loese_mit_ruecksubstitution(A_work, b_work, x_berechnet);

    // 5. Validierung: Wie gut ist unser x_berechnet wirklich?
    printf("[Validator] Starte Residuum-Test...\n");
    berechne_residuum(A_original, x_berechnet, b_original);

    // 6. Aufräumen
    freigabe_dichte_matrix(A_work);
}




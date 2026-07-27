//
// Created by Nia on 15.05.26.
//

#include "matrixstruktur.h"





//Änderung: Transformationsvektor
//Funktion für reine ZSF also Gauss vorwärts
void bringe_in_zeilenstufenform(DichteMatrix dichteMatrix, double b[]) {

    //i Läuft über pivot zeile
    for (int i = 0; i < dichteMatrix.N-1; i++) {
        for (int j = i+1; j < dichteMatrix.N; j++) {

            // Wie oft pivot wert in den zielwert passt
            double faktor = dichteMatrix.daten[j][i] / dichteMatrix.daten[i][i];

            b[j] -= b[i] * faktor;

            //Subtraktion der oberen zeile k mit i
            //bei j=k starten weil links sowieso alles 0
            for (int k = i; k < dichteMatrix.N; k++) {
                dichteMatrix.daten[j][k] -= dichteMatrix.daten[i][k]*faktor;
            }

        }

    }
}




//Rücksubstitution
void loese_mit_ruecksubstitution(DichteMatrix A, double b[], double x[]) {

    // A ist bereits in Zeilenstufenform!
    int N = A.N;

    // Wir laufen von der letzten Zeile rückwärts bis zur ersten Zeile .
    // weil  wir in der letzten Zeile mit nur einer Unbekannten starten
    for (int i = N - 1; i >= 0; i--) {

        // Diese Variable sammelt die Produkte der bereits berechneten x-Werte
        // mit den entsprechenden Matrix-Koeffizienten der aktuellen Zeile i.
        double summe = 0.0;
        // Summiere die bereits bekannten x-Werte auf
        for (int j = i + 1; j < N; j++) {
            summe += A.daten[i][j] * x[j];
        }
        // x[i] = (b[i] - Summe) / Diagonalelement
        x[i] = (b[i] - summe) / A.daten[i][i];
    }
}


















//gen mat ( d1 d2 d3 d4 und k) für bblocke
//mit RB formAT
//DÜNNBESETZER GAUSS ..... wo nicht null elemente und dann handlung nur wo nicht null auch in zukunft

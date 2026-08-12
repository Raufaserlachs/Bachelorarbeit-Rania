//
// Created by Nia on 27.07.26.
//
#include <stdio.h>
#include <math.h>

void vergleiche_loesungen(double x_dicht[], double x_csr[], int N) {
    double fehler_dicht_summe = 0.0;
    double fehler_csr_summe = 0.0;
    double max_abweichung_dicht = 0.0;
    double max_abweichung_csr = 0.0;
    double exakter_wert = 1.0;

    for (int i = 0; i < N; i++) {
        double abw_dicht = fabs(x_dicht[i] - exakter_wert);
        double abw_csr = fabs(x_csr[i] - exakter_wert);

        fehler_dicht_summe += abw_dicht * abw_dicht;
        fehler_csr_summe += abw_csr * abw_csr;

        if (abw_dicht > max_abweichung_dicht) max_abweichung_dicht = abw_dicht;
        if (abw_csr > max_abweichung_csr) max_abweichung_csr = abw_csr;
    }

    double euklid_dicht = sqrt(fehler_dicht_summe);
    double euklid_csr = sqrt(fehler_csr_summe);

    printf("========================================\n");
    printf("        LÖSUNGSVERGLEICH (ZAPFEN/SOLL: 1.0)\n");
    printf("========================================\n");
    printf("Dichter Solver - Euklidischer Fehler: %e\n", euklid_dicht);
    printf("Dichter Solver - Maximale Abweichung: %e\n", max_abweichung_dicht);
    printf("----------------------------------------\n");
    printf("CSR Solver    - Euklidischer Fehler: %e\n", euklid_csr);
    printf("CSR Solver    - Maximale Abweichung: %e\n", max_abweichung_csr);
    printf("========================================\n");

    if (euklid_csr < euklid_dicht) {
        printf("[Ergebnis] Der CSR-Solver ist näher an der exakten Lösung!\n");
    } else if (euklid_dicht < euklid_csr) {
        printf("[Ergebnis] Der dichte Solver ist näher an der exakten Lösung!\n");
    } else {
        printf("[Ergebnis] Beide Solver haben exakt dieselbe Abweichung.\n");
    }
}
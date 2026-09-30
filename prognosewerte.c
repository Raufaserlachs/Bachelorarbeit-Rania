//
// Created by Nia on 18.09.26.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void berechne_dreisatz_hochrechnung(void) {
    // Referenz-Messwerte aus deinen Quelldaten (6x6x6x6x6 -> N = 7.776 Knoten)
    const double N_ref               = 7776.0;

    // CSR-Zeiten (in Sekunden)
    const double t_csr_symb_ref      = 0.771216;     // Symbolische Phase
    const double t_csr_nosymb_ref    = 133.555611;   // Ohne symbolische Phase
    const double t_csr_tot_ref       = 134.326827;   // Insgesamt mit symbolischer Phase

    // Naiver Solver Zeit (in Sekunden)
    const double t_naiv_ref          = 451.413036;   // Gesamte Solver-Laufzeit Naiv

    // FLOPs / Rechenaufwand
    const double flops_csr_ref       = 23166252.0;     // FLOPs CSR gesamt
    const double flops_naiv_ref      = 313607812752.0; // FLOPs Naiv gesamt

    // Array mit allen 5 Ziel-Gittergrößen (Klammern [] sind da!)
    int schritte[] = {10, 20, 30, 40, 50};

    printf("=============================================================================================================================================\n");
    printf("  ERWEITERTE DREISATZ-HOCHRECHNUNG (REFERENZ: n=6 -> N = 7.776 KNOTEN)\n");
    printf("  Formel: Wert_neu = Wert_ref * (N_neu / N_ref)\n");
    printf("=============================================================================================================================================\n\n");

    // Teil 1: Laufzeiten (CSR aufgeteilt + Naiv)
    printf("1. LAUFZEITEN (CSR AUFGETEILT VS. NAIV)\n");
    printf("---------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("%-16s %-10s | %-16s | %-18s | %-18s | %-22s\n",
           "Gitter (n^5)", "N (Knoten)", "t_CSR Symb.", "t_CSR ohne Symb.", "t_CSR Gesamt", "t_Naiv Gesamt");
    printf("---------------------------------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < 5; i++) {
        int n = schritte[i];
        double N = (double)n * n * n * n * n; // N = n^5
        double faktor = N / N_ref;            // Dreisatz-Faktor

        double t_symb   = t_csr_symb_ref * faktor;
        double t_nosymb = t_csr_nosymb_ref * faktor;
        double t_tot    = t_csr_tot_ref * faktor;
        double t_naiv   = t_naiv_ref * faktor;

        printf("%2dx%2dx%2dx%2dx%-2d %-10.0f | %12.2f s | %14.2f s | %10.2f s (%4.1fM) | %10.2f s (%4.1fStd)\n",
               n, n, n, n, n, N,
               t_symb, t_nosymb,
               t_tot, t_tot / 60.0,
               t_naiv, t_naiv / 3600.0);
    }

    printf("\n");

    // Teil 2: Rechenaufwand (FLOPs CSR vs. FLOPs Naiv)
    printf("2. RECHENAUFWAND (FLOPs DREISATZ)\n");
    printf("---------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("%-16s %-10s | %-10s | %-22s | %-22s\n",
           "Gitter (n^5)", "N (Knoten)", "Faktor", "FLOPs CSR (Dreisatz)", "FLOPs Naiv (Dreisatz)");
    printf("---------------------------------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < 5; i++) {
        int n = schritte[i];
        double N = (double)n * n * n * n * n;
        double faktor = N / N_ref;

        double flops_csr  = flops_csr_ref * faktor;
        double flops_naiv = flops_naiv_ref * faktor;

        printf("%2dx%2dx%2dx%2dx%-2d %-10.0f | %-10.2f | %22.0f | %22.0f\n",
               n, n, n, n, n, N, faktor,
               flops_csr, flops_naiv);
    }

    printf("=============================================================================================================================================\n");
}

//
// Created by Nia on 19.09.26.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void berechne_realistische_prognose(void) {
    // ==================================================================================
    // 1. EMPIRISCHE REFERENZMESSWERTE (aus der Quelle "erste Messung")
    // ==================================================================================
    // Messpunkt 1: n = 4 -> N1 = 1.024 Knoten
    const double N1             = 1024.0;
    const double t_csr_symb_1   = 0.016823;
    const double t_csr_zsf_1    = 0.366721;
    const double t_naiv_tot_1   = 1.110770;
    const double flops_csr_1    = 806640.0;
    const double flops_naiv_1   = 718448128.0;

    // Messpunkt 2 (Referenz-Anker): n = 6 -> N_ref = 7.776 Knoten
    const double N_ref               = 7776.0;
    const double t_csr_symb_ref      = 0.771216;      // Symbolische Phase
    const double t_csr_zsf_ref       = 133.507785;    // Numerische ZSF (ohne symb. Phase)
    const double t_naiv_tot_ref      = 451.413036;    // Naiver Solver gesamt
    const double flops_csr_ref       = 23166252.0;    // FLOPs CSR gesamt
    const double flops_naiv_ref      = 313607812752.0;// FLOPs Naiv gesamt

    // ==================================================================================
    // 2. EMPIRISCHE EXPONENTEN (alpha = log(Y_ref / Y1) / log(N_ref / N1))
    // ==================================================================================
    const double log_N_ratio = log(N_ref / N1);

    const double alpha_naiv_time  = log(t_naiv_tot_ref / t_naiv_tot_1) / log_N_ratio; // ~ 2.96 (kubisch)
    const double alpha_naiv_flops = log(flops_naiv_ref / flops_naiv_1) / log_N_ratio;   // ~ 3.00 (kubisch)

    const double alpha_csr_symb   = log(t_csr_symb_ref / t_csr_symb_1) / log_N_ratio; // ~ 1.89
    const double alpha_csr_zsf    = log(t_csr_zsf_ref / t_csr_zsf_1)   / log_N_ratio; // ~ 2.91
    const double alpha_csr_flops  = log(flops_csr_ref / flops_csr_1)   / log_N_ratio; // ~ 1.66

    // Array mit allen 5 Ziel-Gittergrößen (10^5 bis 50^5)
    const int schritte[] = {10, 20, 30, 40, 50};

    printf("=============================================================================================================================================\n");
    printf("  REALISTISCHE SKALIERUNGSPROGNOSE AUF BASIS DER MESSREIHE (REFERENZ: n=6 -> N = 7.776 KNOTEN)\n");
    printf("  Formel: Wert_neu = Wert_ref * (N_neu / N_ref)^alpha\n");
    printf("  Exponenten aus Messung: Naiv-Time=%.2f, Naiv-FLOPs=%.2f, CSR-Symb=%.2f, CSR-ZSF=%.2f, CSR-FLOPs=%.2f\n",
           alpha_naiv_time, alpha_naiv_flops, alpha_csr_symb, alpha_csr_zsf, alpha_csr_flops);
    printf("=============================================================================================================================================\n\n");

    // Teil 1: Laufzeiten
    printf("1. LAUFZEITEN (REALISTISCHE KOMPLEXITAEET: CSR PHASEN VS. NAIV)\n");
    printf("---------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("%-12s %-12s | %-14s | %-16s | %-24s | %-24s\n",
           "Gitter (n^5)", "N (Knoten)", "t_CSR Symb.", "t_CSR Numerisch", "t_CSR Gesamt", "t_Naiv Gesamt");
    printf("---------------------------------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < 5; i++) {
        int n = schritte[i];
        double N = pow((double)n, 5.0); // N = n^5
        double N_ratio = N / N_ref;

        // Zeit-Prognosen über individuelle Potenzgesetze
        double t_symb   = t_csr_symb_ref * pow(N_ratio, alpha_csr_symb);
        double t_nosymb = t_csr_zsf_ref  * pow(N_ratio, alpha_csr_zsf);
        double t_tot    = t_symb + t_nosymb;
        double t_naiv   = t_naiv_tot_ref * pow(N_ratio, alpha_naiv_time);

        printf("%d^5         %-12.0f | %12.2f s | %14.2f s | %10.2f s (%6.1fMin) | %12.2e s (%8.1fStd)\n",
               n, N,
               t_symb, t_nosymb,
               t_tot, t_tot / 60.0,
               t_naiv, t_naiv / 3600.0);
    }

    printf("\n");

    // Teil 2: Rechenaufwand (FLOPs)
    printf("2. RECHENAUFWAND (FLOPs POTENZGESETZ-PROGNOSE)\n");
    printf("---------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("%-12s %-12s | %-12s | %-24s | %-26s | %-12s\n",
           "Gitter (n^5)", "N (Knoten)", "Skal. Naiv", "FLOPs CSR (Realistisch)", "FLOPs Naiv (Realistisch)", "Einsparung");
    printf("---------------------------------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < 5; i++) {
        int n = schritte[i];
        double N = pow((double)n, 5.0);
        double N_ratio = N / N_ref;

        double flops_csr  = flops_csr_ref  * pow(N_ratio, alpha_csr_flops);
        double flops_naiv = flops_naiv_ref * pow(N_ratio, alpha_naiv_flops);
        double einsparung = flops_naiv / flops_csr;

        printf("%d^5         %-12.0f | %12.2f | %24.2e | %26.2e | %12.0fx\n",
               n, N,
               pow(N_ratio, alpha_naiv_flops),
               flops_csr, flops_naiv, einsparung);
    }

    printf("=============================================================================================================================================\n");
}





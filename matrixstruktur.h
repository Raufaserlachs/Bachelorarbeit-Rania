//
// Created by rania on 23.04.26.
//

#ifndef UNTITLED_MATRIXSTRUKTUR_H
#define UNTITLED_MATRIXSTRUKTUR_H

    //einträge
    typedef struct {
        int i;      // Zeilen-Index
        int j;      // Spalten-Index
        double wert;   // Der Wert (1)

    } MatrixEintrag;




    //dünnbesetzte matrix und array aus einträgen
    typedef struct {
        int d1, d2, d3, d4;
        int knotenAnzahl;
        int nne;
        int B;


        //  ein Array aus Einträgen
        MatrixEintrag *eintraege;

    } FlexibleSparseMatrix;




    // CSR Datenstruktur
    typedef struct {
        int N;              // Dimension der Matrix (N x N)
        int nnz;            // Anzahl der Nicht-Null-Elemente
        double *val;        // Werte der Nicht-Null-Elemente
        int *ci;            // Spaltenindizes
        int *rst;           // Row Start (Größe N + 1)
    } CSRMatrix;


    CSRMatrix konvertiere_zu_csr(FlexibleSparseMatrix sparse);
    void freigabe_csr_matrix(CSRMatrix A);
    void drucke_csr_matrix(CSRMatrix A);


    double berechne_norm(double *r, int N);





    FlexibleSparseMatrix konstruiere_flexible_matrix( int d1, int d2, int d3, int d4, int B);
    void drucke_flexible_matrix_tabelle(FlexibleSparseMatrix m);

    //für eine dichte matrix
    typedef struct {
        int N;          // Dimension (N x N)
        double **daten; // Das eigentliche 2D-Array
    } DichteMatrix;


extern long long op_count_zsf;       // Zähler für Zeilenstufenform (Vorwärts-Elimination)
extern long long op_count_ruecksub;  // Zähler für Rücksubstitution


    DichteMatrix konvertiere_zu_dicht(FlexibleSparseMatrix sparse);
    void bringe_in_zeilenstufenform(DichteMatrix dichteMatrix, double b[]);
    void drucke_dichte_matrix(DichteMatrix dichteMatrix);
    void loese_mit_ruecksubstitution(DichteMatrix A, double b[], double x[]);
    DichteMatrix kopiere_matrix(DichteMatrix A);
    void freigabe_dichte_matrix(DichteMatrix A);
    void testlauf(DichteMatrix A, double *x_berechnet_out);
    void test_csr_konvertierung();
    CSRMatrix konvertiere_zu_optimierten_csr(FlexibleSparseMatrix sparse);
    void drucke_optimierte_csr(CSRMatrix A);
    void freigabe_csr_matrix(CSRMatrix A);
    void bringe_in_zeilenstufenform_csr(CSRMatrix A, double b[]);
    double*erstelle_loesungsvektor(int N);
    void loese_rueckwaertssubstitution_csr(CSRMatrix A, double b[], double x[]);
    void drucke_vektor_x(double x[], int N);
    void testlauf_csr(CSRMatrix A_original, double *x_berechnet_out);
    void vergleiche_loesungen(double x_dicht[], double x_csr[], int N);
    void mmvp_csr(CSRMatrix A, double *x, double *b);
    void messe_performance(FlexibleSparseMatrix sparse);
    void messe_aufwand(FlexibleSparseMatrix sparse);




#endif //UNTITLED_MATRIXSTRUKTUR_H

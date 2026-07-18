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




    typedef struct {
        int N;        // Anzahl der Blöcke
        int B;        // Blockgröße (z.B. 2, 4, 8)
        int *row_ptr; // Zeiger auf den Start der Block-Zeilen (Größe N+1)
        int *col_idx; // Spalten-Index der Blöcke (Größe Anzahl_Block_Einträge)
        double *val;  // Daten: Block_Einträge * (B * B)
    } BCSRMatrix;




    DichteMatrix konvertiere_zu_dicht(FlexibleSparseMatrix sparse);
    void bringe_in_zeilenstufenform(DichteMatrix dichteMatrix, double b[]);
    void drucke_dichte_matrix(DichteMatrix dichteMatrix);
    void loese_mit_ruecksubstitution_dichte(DichteMatrix A, double b[], double x[]);
    DichteMatrix kopiere_matrix(DichteMatrix A);
    void freigabe_dichte_matrix(DichteMatrix A);
    void testlauf(DichteMatrix A);
    void test_csr_konvertierung();
    CSRMatrix konvertiere_zu_optimierten_csr(FlexibleSparseMatrix sparse);
    void drucke_optimierte_csr(CSRMatrix A);
    void bringe_in_zeilenstufenform(DichteMatrix dichteMatrix, double b[]);
    void loese_mit_ruecksubstitution_csr(CSRMatrix A, double b[], double x[]);
    void drucke_csr_nach_gauss(CSRMatrix A);
BCSRMatrix konvertiere_zu_bcsr(FlexibleSparseMatrix sparse);
void invert_block(double *mat, int B);
void block_mul(double *a, double *b, double *res, int B);
void bcsr_sub_mul(double *target, double *factor, double *block, int B);
void set_value_csr(CSRMatrix A, int row, int col, double val);
void bringe_in_zeilenstufenform_bcsr(BCSRMatrix A, double *b);
void drucke_bcsr_nach_gauss(BCSRMatrix A);
void drucke_bcsr_vor_gauss(BCSRMatrix A);
void freigeben_bcsr_matrix(BCSRMatrix *A);


    void testlauf_csr(CSRMatrix A);
    void freigabe_csr_matrix(CSRMatrix A);






#endif //UNTITLED_MATRIXSTRUKTUR_H

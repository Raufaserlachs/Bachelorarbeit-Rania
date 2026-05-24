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

        //  ein Array aus Einträgen
        MatrixEintrag *eintraege;

    } FlexibleSparseMatrix;


    FlexibleSparseMatrix konstruiere_flexible_matrix( int d1, int d2, int d3, int d4 );
    void print_flexible_matrix_tabelle(FlexibleSparseMatrix m);

    //für eine dichte matrix
    typedef struct {
        int N;          // Dimension (N x N)
        double **daten; // Das eigentliche 2D-Array
    } DichteMatrix;

    DichteMatrix konvertiere_zu_dicht(FlexibleSparseMatrix sparse);
    void bringe_in_zeilenstufenform(DichteMatrix dichteMatrix);
    void drucke_dichte_matrix(DichteMatrix dichteMatrix);

#endif //UNTITLED_MATRIXSTRUKTUR_H

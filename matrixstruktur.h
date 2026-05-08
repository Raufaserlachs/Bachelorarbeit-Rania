//
// Created by rania on 23.04.26.
//

#ifndef UNTITLED_MATRIXSTRUKTUR_H
#define UNTITLED_MATRIXSTRUKTUR_H


    typedef struct {
        int i;      // Zeilen-Index
        int j;      // Spalten-Index
        double wert;   // Der Wert (1)

    } MatrixEintrag;

    typedef struct {
        int d1, d2, d3, d4;
        int knotenAnzahl;
        int nne;

        //  ein Array aus Einträgen
        MatrixEintrag *eintraege;

    } FlexibleSparseMatrix;



    FlexibleSparseMatrix konstruiere_flexible_matrix( int d1, int d2, int d3, int d4 );
    void print_flexible_matrix_tabelle(FlexibleSparseMatrix m);



#endif //UNTITLED_MATRIXSTRUKTUR_H

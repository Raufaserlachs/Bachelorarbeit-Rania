//
// Created by rania on 23.04.26.
//

#ifndef UNTITLED_MATRIXSTRUKTUR_H
#define UNTITLED_MATRIXSTRUKTUR_H


typedef struct {
        int n; //Dimension n x n Matrix
        int nne; // Anzahl nicht null elemente um später Speicher zu reservieren
        double *wert; //für echte werte später
        int *spaltenIndex;
        int *zeilenIndex;

    } SparseMatrix ;

    SparseMatrix konstruiere_matrix();
    void loesche_matrix( SparseMatrix *matrix);
    void print_matrix(SparseMatrix matrix);

#endif //UNTITLED_MATRIXSTRUKTUR_H

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

    typedef struct {
        //Dimmension
        int d1;
        int d2;
        int d3;
        int d4;

        int knotenAnzahl;
        int nne; // Anzahl der Verbindungen (Kanten)
        
        //Array
        // CSR-Arrays
        int *wert;           // Gewichte (alle 1.0)
        int *spalten_indizes;   // Welcher Knoten ist der Nachbar?
        int *zeilen_zeiger;     // Wo fangen die Nachbarn eines Knotens an?

    } FlexibleSparseMatrix;

    SparseMatrix konstruiere_matrix();
    void loesche_matrix( SparseMatrix *matrix);
    void print_matrix(SparseMatrix matrix);

    FlexibleSparseMatrix konstruiere_flexible_matrix();
    void loesche_flexible_matrix( FlexibleSparseMatrix *matrix);
    void print_flexivle_matrix(FlexibleSparseMatrix matrix);




#endif //UNTITLED_MATRIXSTRUKTUR_H

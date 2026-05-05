//
// Created by rania on 23.04.26.
//

#ifndef UNTITLED_MATRIXSTRUKTUR_H
#define UNTITLED_MATRIXSTRUKTUR_H


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
        int *wert;           // Gewichte
        int *spalten_indizes;   // Welcher Knoten ist der Nachbar?
        int *zeilen_zeiger;     // Wo fangen die Nachbarn eines Knotens an?

    } FlexibleSparseMatrix;



    FlexibleSparseMatrix konstruiere_flexible_matrix( int d1, int d2, int d3, int d4 );
    void print_flexible_matrix_tabelle(FlexibleSparseMatrix m);



#endif //UNTITLED_MATRIXSTRUKTUR_H

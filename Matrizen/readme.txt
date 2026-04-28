Mit gzip -d können *.gz Dateien dekomprimiert werden. ACHTUNG, A8444.txt.gz wird mit ~11GB sehr groß werden, das Kommando zum dekomprimieren dauert entsprechend lange.

Die A2222.txt Datei (~120MB) enthält die Matrix für ein Gitter von 2^4 Gitterpunkten (2 Punkte in jeder Raum/Zeit Dimension). 
Die A8444.txt Datei (~11GB) enthält die Matrix für ein Gitter von 8x4^3 Gitterpunkten (4 Punkte in jeder Raum Dimension x 8 Punkte in Zeit).
(A8444.txt.gz nicht im Git, da 119MB die Git-Datei-Größe überschreitet.) 

Beide Matrizen bestehen aus Blöcken der Größe 56. Entsprechend erstreckt sich der erste nicht-Null-Block von Matrix Koordinate (0,0) über (56,0), (0,56) bis (56,56).


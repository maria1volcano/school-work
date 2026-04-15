#ifndef QUANTIFICATION_H
#define QUANTIFICATION_H

#include "histogram.h"
#include "image.h"

/*  Remplit tab avec les K couleurs les plus fréquentes */
void quantification(histo h, int* tab, int K);

/* Crée l'image de sortie en remplaçant les pixels de l'image d'entrée par la couleur la plus proche trouvée dans tab. */
void mapping(image input, image output, int* tab, int K);

#endif

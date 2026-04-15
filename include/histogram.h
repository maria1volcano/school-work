#ifndef HISTOGRAM_H
#define HISTOGRAM_H
#include "image.h" /* Nécessaire pour le type 'image' utilisé dans init_histo */

/* Définition du booléen comme demandé */
typedef enum { false, true } boolean;

/* Définition  de la structure cell (le contenu est écrit dans le histogram.c) */
typedef struct cell * cell;

/* Définition du type histo (Tableau 2D de listes) */
typedef cell * histo; 

/* Définition  de l'itérateur */
typedef struct histo_iter * histo_iter;

/* 2.1 Manipulation de listes */
cell create_cell(int B, cell next);
cell insert_cell(cell head, int B);
cell delete_list(cell list);

/* 2.2 Gestion de l'histogramme */
histo create_histo();
void init_histo(histo h, image img);
void delete_histo(histo h);

/* 2.3 Interrogation de l'histogramme */
int give_freq_histo(histo h, int R, int G, int B);

/* Gestion de l'itérateur lié à l'histogramme  */
histo_iter create_histo_iter(histo h);
void start_histo_iter(histo_iter it);
boolean next_histo_iter(histo_iter it);
void give_color_histo_iter(histo_iter it, int* couleur);
void delete_histo_iter(histo_iter it);

#endif

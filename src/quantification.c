
#include <stdio.h>
#include <stdlib.h>
#include "quantification.h"

/* On défini une nouvelle structure pour obtenir notre liste chaînée des K plus fréquentes couleurs */
typedef struct cell_color  {
    int R;
    int G;
    int B;
    int freq;
    struct cell_color* next;
} *cell_color;


void quantification(histo h, int* tab, int K) {

    histo_iter iter;
    cell_color head;
    cell_color tail; /* On garde le pointeur sur le dernier élément */
    cell_color curr;
    cell_color prev;
    cell_color new;
    cell_color temp;
    int color[3];
    int freq;
    int nb_elements;
    int i;

    head = NULL;
    tail = NULL;
    nb_elements = 0;
    iter = create_histo_iter(h);

    /* On test chaque couleur de l'image*/
    do {
        /* On récupère les informations concernant la couleur actuelle */
        give_color_histo_iter(iter, color);
        freq = give_freq_histo(h, color[0], color[1], color[2]);



        /* Si la liste est pleine et que la fréquence actuelle est <= au dernier de la liste,
           on ne perd pas de temps, on passe au suivant. */
        if (nb_elements == K && tail != NULL) {
            if (freq <= tail->freq) {
                continue;
            }
        }

        /* Si on est ici, c'est qu'on doit insérer une nouvelle cellule (et potentiellement en libérer une) */

        /* Allocation et création de la nouvelle cellule */
        new = malloc(sizeof(struct cell_color));
        if (new == NULL) {
            exit(1);
        }

        new->R = color[0];
        new->G = color[1];
        new->B = color[2];
        new->freq = freq;

        /* Recherche de la place de notre cellule */
        curr = head;
        prev = NULL;
        while (curr != NULL && curr->freq >= freq) {
            prev = curr; /*prev sera la cellule précédant la nouvelle*/
            curr = curr->next; /*curr arrivera après notre nouvelle cellule*/
        }

        /* Insertion */
        new->next = curr;
        if (prev == NULL) { /*On vérifie si l'insertion ne se fait pas en tête de liste*/
            head = new;
        } else {
            prev->next = new;
        }

        /* Si on insère tout à la fin, cette nouvelle cellule devient le tail. */
        if (new->next == NULL) {
            tail = new;
        }

        nb_elements++;

        /* Si on dépasse K cellules, on supprime la dernière */
        if (nb_elements > K) {
            /* On doit trouver l'avant-dernière pour libérer la dernière proprement.*/
            curr = head;
            for (i = 0; i < K - 1; i++) {
                curr = curr->next;
            }

            temp = curr->next;

            free(temp);
            curr->next = NULL;

            /* On met à jour le pointeur tail sur le nouveau dernier ! */
            tail = curr;

            nb_elements = K;
        }

    } while (next_histo_iter(iter));

    /* On transfert les informations de la liste chaînée vers le tableau */
    curr = head;
    for (i = 0; i < K; i++) {
        if (curr != NULL) {
            tab[3 * i]     = curr->R;
            tab[3 * i + 1] = curr->G;
            tab[3 * i + 2] = curr->B;

            /* Nettoyage mémoire au passage */
            temp = curr;
            curr = curr->next;
            free(temp);
        } else {
            tab[3 * i] = 0;
            tab[3 * i + 1] = 0;
            tab[3 * i + 2] = 0;
        }
    }

    delete_histo_iter(iter);
}

/* On introduis une fonction interne qui va nous permettre de manipuler des calculs de distance de façon plus simple pour la suite */
static long dist_sq(int R1, int G1, int B1, int R2, int G2, int B2) {
    long dR = R1 - R2;
    long dG = G1 - G2;
    long dB = B1 - B2;
    return dR * dR + dG * dG + dB * dB;
}

/* Première version naïve */
void mapping(image input, image output, int* tab, int K) {
    int *pixel_in;    /* Le pixel qu'on lit (R,G,B) */
    int pixel_out[3]; /* Le pixel qu'on va écrire */
    int w;
    int h;

    int i_pixel;
    int nb_pixels;

    int k;
    int best_k;       /* L'indice de la meilleure couleur trouvée */

    long current_dist;
    long min_dist;

    /* Récupération des dimensions */
    w = image_give_largeur(input);
    h = image_give_hauteur(input);
    nb_pixels = w * h;

    /* On se place au début des deux images */
    image_debut(input);
    image_debut(output);

    /* Boucle sur TOUS les pixels de l'image */
    for (i_pixel = 0; i_pixel < nb_pixels; i_pixel++) {

        /* On lit d'abord le pixel de l'image d'entrée */
        pixel_in = image_lire_pixel(input);

        /* On essaye de trouver la couleur la plus proche dans tab  */
        min_dist = -1; /* -1 signifiera "pas encore initialisé" */
        best_k = 0;

        for (k = 0; k < K; k++) {
            /* Calcul de distance entre le pixel image et la couleur k du tableau */
            current_dist = dist_sq(pixel_in[0], pixel_in[1], pixel_in[2],
                                   tab[3 * k], tab[3 * k + 1], tab[3 * k + 2]);

            /* Si c'est la première qu'on teste ou si c'est la plus proche on la garde */
            if (min_dist == -1 || current_dist < min_dist) {
                min_dist = current_dist;
                best_k = k;
            }
        }

        /* On remplace les pixels de sortie avec ceux de la meilleure couleur trouvée */
        pixel_out[0] = tab[3 * best_k];
        pixel_out[1] = tab[3 * best_k + 1];
        pixel_out[2] = tab[3 * best_k + 2];

        image_ecrire_pixel(output, pixel_out);


        image_pixel_suivant(input);
        image_pixel_suivant(output);
    }
}

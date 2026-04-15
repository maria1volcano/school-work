#include <stdio.h>
#include <stdlib.h>

#include "image.h"
#include "histogram.h"

int main (int argc, char *argv[]){
    image img;
    histo h;
    /*on utilise l'itérateur pour */
    
    histo_iter it; 
    int couleur_tampon[3];
    int r;
    int g;
    int b;
    int freq;
    int lum;

    if (argc != 2){
        fprintf(stderr," usage: %s image.ppm\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Creation et chargement de l image */
    img = FAIRE_image();
    if (image_charger(img, argv[1]) != 0){
        fprintf(stderr, "erreur chargement de l image \n");
        DEFAIRE_image(img);
        return EXIT_FAILURE;
    }

    /* Construction de l'histogramme */
    h = create_histo();
    init_histo(h, img);

   
    /* Initialisation de l'itérateur (pointe sur la première couleur valide) */
    it = create_histo_iter(h); 
    
    start_histo_iter(it);
    /* Parcours tant que l'itérateur renvoie une couleur valide.
       La fonction remplit les variables r, g, b et freq par adresse. */
    while (next_histo_iter(it)==true) {
        
        /* A. On récupère la couleur courante (R, G, B) */
        give_color_histo_iter(it, couleur_tampon);
        r = couleur_tampon[0];
        g = couleur_tampon[1];
        b = couleur_tampon[2];

        /* B. On récupère la fréquence via ta fonction d'interrogation */
        freq = give_freq_histo(h, r, g, b);

        lum = (r + g + b) / 3;

        printf("%d %d %d %d %d\n", r, g, b, lum, freq);
    }
    delete_histo_iter(it);
    delete_histo(h);
    DEFAIRE_image(img);

    return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "histogram.h"
#include "quantification.h"


int main(int argc, char *argv[]) {


    image input;
    image output;
    histo h;
    int K;
    int *tab_couleurs; /*nous permettra de stocker nos K couleurs*/
    int largeur;
    int hauteur;

    if (argc != 4) {
        printf( "Erreur: Ecrivez %s <image_entree.ppm> <K> <image_sortie.ppm>\n", argv[0]);
        exit(1);
    }

    /* argv[2] est une chaîne de caractères (ex: "16").
      atoi le transforme en entier (int) utilisable pour les calculs. */
    K = atoi(argv[2]);

    if (K <= 0) {
        printf( "Erreur: K doit être un entier positif \n");
        exit(1);
    }

    printf(" Chargement de l'image '%s'...\n", argv[1]);
    input = FAIRE_image();
    if (input == NULL_IMAGE) { /* Sécurité supplémentaire */
        printf( "Erreur : Impossible de créer l'objet image.\n");
        exit(1);
    }
    if (image_charger(input, argv[1]) != 0) {
        printf( "Erreur: Impossible d'ouvrir l'image %s\n", argv[1]);
        DEFAIRE_image(input);
        exit(1);
    }


    printf(" Analyse des couleurs \n");
    h = create_histo();
    init_histo(h, input);

    printf("Sélection des %d couleurs les plus fréquentes...\n", K);

    tab_couleurs = malloc(K * 3 * sizeof(int));
    if (tab_couleurs == NULL) {
        printf( "Erreur d'allocation mémoire.\n");
        delete_histo(h);
        DEFAIRE_image(input);
        exit(1);
    }

    quantification(h, tab_couleurs, K);

    printf(" Remplacement des couleurs. \n");

    largeur = image_give_largeur(input);
    hauteur = image_give_hauteur(input);

    output = FAIRE_image();
    if (output == NULL) {
        printf( "Erreur : Impossible de créer l'image de sortie.\n");

        free(tab_couleurs);
        delete_histo(h);
        DEFAIRE_image(input);
        exit(1);
    }


    image_initialize(output, 3, largeur, hauteur);

    mapping(input, output, tab_couleurs, K);

    printf(" Sauvegarde dans '%s'...\n", argv[3]);
    image_sauvegarder(output, argv[3]);

    /* Nettoyage */
    free(tab_couleurs);
    delete_histo(h);
    DEFAIRE_image(input);
    DEFAIRE_image(output);

    return EXIT_SUCCESS;
}

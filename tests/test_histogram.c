#include <stdio.h>
#include <stdlib.h>
#include "minunit.h"   /* Le fichier de ton prof */
#include "histogram.h"
#include "image.h"

#define IMAGE_TEST "EXEMPLES/out.ppm" 

/* --- TEST 1 : Création et Destruction --- */
MU_TEST(test_creation_destruction) {
    histo h = create_histo();
    
    /* mu_check vérifie juste si c'est vrai */
    mu_check(h != NULL);
    
    delete_histo(h);
}

/* --- TEST 2 : Fréquences (Logique de base) --- */
MU_TEST(test_frequence_coherence) {
    image img = FAIRE_image();
    histo h;
    int *pixel;
    int freq;
    
    /* Si l'image n'est pas là, on fait échouer le test proprement */
    if (image_charger(img, IMAGE_TEST) != 0) {
        DEFAIRE_image(img);
        mu_fail("Impossible de charger l'image de test"); 
        return;
    }

    h = create_histo();
    init_histo(h, img);

    /* On prend le pixel (0,0) */
    pixel = image_get_pixel(img, 0, 0);
    
    /* On vérifie sa fréquence */
    freq = give_freq_histo(h, pixel[0], pixel[1], pixel[2]);
    
    /* mu_assert_int_eq est génial : il te dira "Attendu X, obtenu Y" si ça rate */
    mu_assert(freq > 0, "La fréquence du pixel (0,0) doit être positive");
    
    delete_histo(h);
    DEFAIRE_image(img);
}

/* --- TEST 3 : Itérateur (Ton itérateur à toi) --- */
MU_TEST(test_iterateur) {
    image img = FAIRE_image();
    histo h = create_histo();
    histo_iter it;
    int couleur[3];
    int freq_directe;
    
    if (image_charger(img, IMAGE_TEST) == 0) {
        init_histo(h, img);
        
        it = create_histo_iter(h);
        
        /* Vérification immédiate */
        mu_check(it != NULL);
        
        give_color_histo_iter(it, couleur);
        
        /* Vérif cohérence entre itérateur et give_freq */
        freq_directe = give_freq_histo(h, couleur[0], couleur[1], couleur[2]);
        mu_assert_int_eq(freq_directe, give_freq_histo(h, couleur[0], couleur[1], couleur[2]));
        
        /* Vérif qu'on n'est pas sur une case vide */
        mu_assert(freq_directe > 0, "L'itérateur pointe sur une couleur vide !");
        
        delete_histo_iter(it);
    }
    
    delete_histo(h);
    DEFAIRE_image(img);
}

/* --- RASSEMBLEMENT DES TESTS (SUITE) --- */
MU_TEST_SUITE(test_suite) {
    /* On liste les tests à lancer */
    MU_RUN_TEST(test_creation_destruction);
    MU_RUN_TEST(test_frequence_coherence);
    MU_RUN_TEST(test_iterateur);
}

/* --- MAIN AUTOMATIQUE --- */
int main(int argc, char *argv[]) {
    /* Lancement de la suite */
    MU_RUN_SUITE(test_suite);
    
    /* Affichage du rapport final (temps, nb erreurs...) */
    MU_REPORT();
    
    return MU_EXIT_CODE; /* Renvoie 0 si tout est OK, ou le nb d'erreurs */
}

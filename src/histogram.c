#include <stdio.h>
#include <stdlib.h>
#include "histogram.h"
#include "image.h"

struct cell{
    unsigned char B;
    int freq;
    cell next;
    };

cell create_cell(int B, cell next){
    cell c;
    c=malloc(sizeof(*c));
    if ( c== NULL) return NULL;                 /* test de securite*/
    c -> B = B;
    c ->freq =1;
    c -> next = next;
    return c;
}
cell insert_cell(cell head, int B){
    cell current;
    cell prev;
    if (head == NULL) return create_cell(B, NULL);  /* 1 er cas : liste vide */
    if (B < head ->B) return create_cell(B, head);  /* 2 eme cas : b exite */
    /* on parcourt la liste */
    current=head;
    prev=NULL ;
    while ( current != NULL && current -> B < B){
        prev= current ;
        current= current -> next;
    }
    /* on s'arrete puis on observe la cellule courrante */

    if ( current != NULL && current -> B == B) {
        current -> freq ++ ;
        return head;
    }
    /* si b n existe pas ou exite mais > b prev et < b current ajoutew au milieu */
    prev -> next = create_cell(B,current);
    return head;
}
cell delete_list(cell list){
    cell tmp;  /*eviter fuite de memoire*/
    while ( list!= NULL){
        tmp =list;
        list = list -> next;
        free(tmp);
    }
    return NULL;
}

histo create_histo(void){
    histo h;
    h= calloc(256*256, sizeof(cell));
    return h;
}

void init_histo(histo h , image img){
    unsigned char R;
    unsigned char G;
    unsigned char B;
    int *pixel;
    
    int index;
    image_debut(img);
    while (image_pixel_suivant(img)) {
        pixel=image_lire_pixel(img);
        R=(unsigned char)pixel[0];
        G=(unsigned char)pixel[1];
        B=(unsigned char)pixel[2];

    /*for (i=0; i<img.width * img.height ; i++){
        R= p[3*i];
        G= p[3*i+1];
        B= p[3*i+2];*/
        
        index= R*256 + G; /*provient d ela representation en memoire des
        tableaux en c */ 
        if (h[index] == NULL){

            h[index]=create_cell(B, NULL);
        }
        else {

            h[index] = insert_cell(h[index],B);
        }
    }
}

void delete_histo(histo h){

    int i;
    if (h== NULL) return ;
    for(i=0; i< 256*256; i++){
        h[i] = delete_list(h[i]);
    }
    free(h);
}
void print_histo(histo h){
    int i;
    cell current;
    for(i=0;i<256*256;i++){
        if(h[i] != NULL){
            printf("Index %d (R:%d , G:%d) ->", i,i/256,i%256);
            current=h[i];
            while (current !=NULL){
                printf("[B:%d, freq:%d]", current ->B, current->freq);
                current=current -> next;
             }
             printf("\n");
         }
     }
}

/*PARTIE 2.3 INTERROGRATION DE L'HISTOGRAMME*/

struct histo_iter{
    int R;
    int G;
    cell current;
    histo source; /*AJOUT: nécessaire pour la fonction next_histo_iter*/
    };

/* Retourne la fréquence (nombre d'occurrences) du pixel (R,G,B) en parcourant la liste chaînée associée. Renvoie 0 si la couleur est absente. */

int give_freq_histo(histo h, int R, int G, int B) {
    cell current;
    int index;

    /*On convertit les coordonnées 2D (R,G) en adresse 1D */
    index = R * 256 + G;

    /* On va directement à la bonne case du tableau */
    current = h[index];

    /*On parcourt la liste chaînée  */
    while (current != NULL) {

        /* On a trouvé le Bleu (B) exact ! On renvoie la fréquence */
        if (current->B == B) {
            return current->freq;
        }

        /* Comme la liste est triée (petit B vers grand B),
           si on tombe sur un B plus grand que celui cherché, c'est fini,
           on ne le trouvera jamais plus loin. */
        if (current->B > B) {
            return 0;
        }
        current = current->next;
    }
    return 0;
}


/* Structure interne de l'itérateur : mémorise la position actuelle (R, G) dans le tableau et le pointeur vers la cellule (B) en cours de lecture. */
struct histo_iter_struct {
    int R;             
    int G;             
    cell current;      /* Le pointeur vers le maillon (B) */
    histo source;      /* Le tableau complet */
};


/*AJOUT D'UNE FONCTION SUPPLEMENTAIRE*/
/* Fonction privée : Cherche la prochaine liste non vide à partir d'un index donné.
   Si trouvé : Met à jour R, G et current dans l'itérateur et renvoie 1.
   Si pas trouvé : Renvoie 0. */
static int aller_au_prochain_non_vide(histo_iter iter, int index_depart) {
    int i;
    histo h = iter->source;

    /* On parcourt le tableau linéaire à partir du point demandé */
    for (i = index_depart; i < 256 * 256; i++) {
        if (h[i] != NULL) {
            
            /* On reconvertit l'index 1D en R et G */
            iter->R = i / 256;
            iter->G = i % 256;

            iter->current = h[i]; /* On pointe sur la tête de liste */
            return 1;
        }
    }
    return 0; /* Fin du tableau atteint */
}


/* Alloue et initialise un nouvel itérateur, en le positionnant sur la toute première couleur présente dans l'histogramme. */
histo_iter create_histo_iter(histo h) {
    histo_iter iter;

    iter = malloc(sizeof(struct histo_iter_struct));
    if (iter == NULL) exit(1);

    iter->source = h;

    /* On place l'itérateur au tout début (index 0) */
    /* Le assert sert à planter le programme si l'histo est vide */
    if (aller_au_prochain_non_vide(iter, 0) == 0) {
        printf("Erreur : Tentative de créer un itérateur sur un histogramme vide.\n");
        exit(1);
    }

    return iter;
}

/* Réinitialise un itérateur existant en le replaçant au début de l'histogramme pour recommencer un parcours. */
void start_histo_iter(histo_iter iter) {
    aller_au_prochain_non_vide(iter, 0);
}


/* Déplace l'itérateur vers la prochaine couleur disponible (soit dans la liste courante, soit dans la case suivante du tableau). Renvoie TRUE si une couleur est trouvée, FALSE si la fin est atteinte. */
boolean next_histo_iter(histo_iter iter) {
    int index_actuel;
    /* 1. Est-ce qu'il y a un maillon SUIVANT dans la liste actuelle (même R,G) ? */
    if (iter->current->next != NULL) {
        iter->current = iter->current->next; /* Oui, on avance juste le pointeur */
        return true; /* On a trouvé, on renvoie Vrai */
    }

    /* 2. Non, la liste est finie. Il faut chercher la prochaine case R,G non vide */

    /* Calcul de l'index actuel pour savoir d'où repartir */
    index_actuel = iter->R * 256 + iter->G;

    /* On lance la recherche à la case SUIVANTE (+1) */
    if (aller_au_prochain_non_vide(iter, index_actuel + 1) == 1) {
        return true; /* On a trouvé une nouvelle liste plus loin */
    }

    /* 3. On a tout scanné jusqu'à la fin, il n'y a plus rien */
    return false;
}


/* Remplit le tableau passé en paramètre avec les composantes R, G et B de la couleur actuellement pointée par l'itérateur. */
void give_color_histo_iter(histo_iter iter, int* couleur) {
    /* C'est super simple car on stocke déjà R et G ! */
    couleur[0] = iter->R;
    couleur[1] = iter->G;
    couleur[2] = (int)iter->current->B;
}


/* Libère la mémoire allouée pour la structure de l'itérateur */
void delete_histo_iter(histo_iter iter) {
    if (iter != NULL) free(iter);
}

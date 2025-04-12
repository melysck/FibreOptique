#include "stdio.h"
#include "stdlib.h"
#include "Hachage.h"
#include "math.h"
#include "Liste.h"


/* ~~~~~~ Fonctions ajoutées pour un code modulaire / libération  ~~~~~~ */

void libererTableHachage(TableHachage *H) {
    if (H == NULL) {
        return;
    }

    // Libérer chaque liste de chaînage dans la table de hachage
    for (int i = 0; i < H->tailleMax; i++) {
        CellNoeud *courant = H->T[i];
        while (courant != NULL) {
            CellNoeud *suivant = courant->suiv;
            free(courant);
            courant = suivant;
        }
    }

    // Libérer le tableau de pointeurs
    free(H->T);

    // Libérer la structure de la table de hachage elle-même
    free(H);
}


Noeud * creeNoeud (Reseau * R , double x , double y ){
    Noeud * n = malloc(sizeof(Noeud)); //on crée le noeud et lui attribue les valeurs nécessaires
    n->num = R->nbNoeuds + 1;
    n->x = x;
    n->y = y;
    n->voisins = NULL;

    CellNoeud * cell = malloc(sizeof(CellNoeud)); 
    cell->nd = n; 
    cell->suiv = R->noeuds;
    R->noeuds = cell;
    R->nbNoeuds++;
    return n ;
}


/* ~~~~~~ Fonctions principales ~~~~~~ */


//f (x, y) = y + (x + y)(x + y + 1)/2

double Clef(double x, double y){
    return (y + (x+y)*(x+y+1)/2);
}

int fonctionHachage(int M, double k){
    double A = (sqrt(5)-1)/2;
    return floor(M*((k*A)-floor(k*A)));
}

Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage*
H, double x, double y){
    int index = fonctionHachage(H->tailleMax, Clef(x, y));

    CellNoeud* pos = H->T[index]; //case de la hashtable qui pourrait stocker le noeud de coord (x, y)

    while(pos){ //parcours de la liste des noeuds
        if(pos->nd->x==x && pos->nd->y==y){
            return pos->nd;
        }
        pos = pos->suiv;
    }

    if(!pos){ //Si la liste est finie et on a pas trouvé le noeud
        Noeud * n = creeNoeud(R,x,y);

        CellNoeud * cell = malloc(sizeof(CellNoeud));  
        cell->nd = n; 
        cell->suiv = H->T[index];
        H->T[index] = cell;
        H->nbElement++;

        return n;
    }
    return NULL ;
}   


TableHachage* initialiseTablehachage(int m){

    TableHachage* H = malloc(sizeof(TableHachage));

    CellNoeud **tab = malloc(sizeof(CellNoeud*)*m);
    H->tailleMax = m;
    H->nbElement = 0;
    H->T = tab;


    for(int i = 0; i<m; i++){
        H->T[i] = NULL;
    }
    return H;
}
Reseau* reconstitueReseauHachage(Chaines* C, int M) {
    Reseau *R = (Reseau*)malloc(sizeof(Reseau));
    R->nbNoeuds = 0;
    R->gamma = C->gamma;
    R->noeuds = NULL;
    R->commodites = NULL;
    // Création de la table de hachage
    TableHachage *H = initialiseTablehachage(M);

    CellChaine *chaine = C->chaines;
    while (chaine != NULL) {
        CellPoint *point = chaine->points;
        Noeud *prec_noeud = NULL;
    
        while (point != NULL) {
            Noeud *noeud = rechercheCreeNoeudHachage(R, H, point->x, point->y);
            // si c'est pas le premier noeud
            if (prec_noeud != NULL) {
                // Ajouter noeud à la liste des voisins de prec_noeud
                construireLiaison ( noeud , prec_noeud);
                //Ajouter prec_noeud à la liste des voisins de noeud pour la liaison bidirectionnelle
                construireLiaison ( prec_noeud , noeud);
                
            }

            prec_noeud = noeud;
            point = point->suiv;
        }

        // Ajouter les commodités de la chaîne à R->commodites 
        CellCommodite *nv_commodite = (CellCommodite*)malloc(sizeof(CellCommodite));
        nv_commodite->extrA = rechercheCreeNoeudHachage(R, H, chaine->points->x, chaine->points->y);
        nv_commodite->extrB = prec_noeud;
        nv_commodite->suiv = R->commodites;
        R->commodites = nv_commodite;

        chaine = chaine->suiv;
    }

    // Libérer la mémoire utilisée par la table de hachage
    libererTableHachage(H);

    return R;
}

#include <stdio.h>
#include "ArbreQuat.h"
#include "Chaine.h"
#include "Reseau.h"
#include "stdlib.h"






/* ~~~~~~ Fonctions ajoutées pour un code modulaire / libération  ~~~~~~ */


Noeud * creeNoeudAQ (Reseau *R ,double x , double y ){ 
    //ici on aurait pu directement utiliser rechercheCreeNoeudListe mais j'ai préféré ajouter une nouvelle fonction (comme on veut comparer ce qui est le plus optimale entre ces fonctions )

    Noeud * n_nv = malloc(sizeof(Noeud));
    n_nv ->num = R->nbNoeuds +1;
    n_nv->x=x;
    n_nv->y=y;
    n_nv->voisins=NULL;

    CellNoeud *nouvelleCellule = malloc(sizeof(CellNoeud));
    nouvelleCellule->nd = n_nv ;
    nouvelleCellule->suiv=R->noeuds;
    R->noeuds=nouvelleCellule;
    
    R->nbNoeuds=R->nbNoeuds +1;

    return n_nv;

}


void liberer_ArbreQuat(ArbreQuat* aq){
    if(aq == NULL){
        return;
    }

    liberer_ArbreQuat(aq-> so);
    liberer_ArbreQuat(aq-> se);
    liberer_ArbreQuat(aq-> no);
    liberer_ArbreQuat(aq-> ne);

    free(aq);
}









/* ~~~~~~ Fonctions principales ~~~~~~ */

void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax, double* ymax){
    
    CellChaine* tmp_chaine = C-> chaines;
    CellPoint* tmp_point;

    while(tmp_chaine != NULL){
        tmp_point = tmp_chaine-> points;

        while(tmp_point != NULL){

            if((*xmin) > (tmp_point-> x)) *xmin = (tmp_point-> x);
            if((*ymin) > (tmp_point-> y)) *ymin = (tmp_point-> y);

            if((tmp_point-> x) > (*xmax)) *xmax = (tmp_point-> x);
            if((tmp_point-> y) > (*ymax)) *ymax = (tmp_point-> y);

            tmp_point = tmp_point-> suiv;
        }

        tmp_chaine = tmp_chaine-> suiv;
    }
}

ArbreQuat* creerArbreQuat(double xc, double yc, double coteX, double coteY) {
    ArbreQuat* arbre = (ArbreQuat*)malloc(sizeof(ArbreQuat));
    if (arbre != NULL) {
        arbre->xc = xc;
        arbre->yc = yc;
        arbre->coteX = coteX;
        arbre->coteY = coteY;
        arbre->no = NULL;
        arbre->ne = NULL;
        arbre->so = NULL;
        arbre->se = NULL;
        arbre->noeud = NULL;
    }
    return arbre;
}



void insererNoeudArbre(Noeud* n, ArbreQuat** a, ArbreQuat* parent){
    
    double xc, yc, coteX, coteY;
    
    // Si l'arbre est vide:
    if(*a == NULL){
        if( ((parent-> xc) > (n-> x)) && ((parent-> yc) > (n-> y)) ){
            
            xc = parent-> xc - (parent-> coteX / 4);
            yc = parent-> yc - (parent-> coteY / 4);
            coteX = parent-> coteX / 2;
            coteY = parent-> coteY / 2;
            *a = creerArbreQuat(xc, yc, coteX, coteY);
            // On insère le noeud.
            (*a)-> noeud = n;

            // On met à jour le pointeur du parent.
            parent-> so = *a;

            return;
        }

        if( ((parent-> xc) <= (n-> x)) && ((parent-> yc) > (n-> y)) ){
            
            xc = parent-> xc + (parent-> coteX / 4);
            yc = parent-> yc - (parent-> coteY / 4);
            coteX = parent-> coteX / 2;
            coteY = parent-> coteY / 2;
            *a = creerArbreQuat(xc, yc, coteX, coteY);
            // On insère le noeud.
            (*a)-> noeud = n;

            // On met à jour le pointeur du parent.
            parent-> se = *a;

            return;
        }

        if( ((parent-> xc) <= (n-> x)) && ((parent-> yc) <= (n-> y)) ){
            
            xc = parent-> xc + (parent-> coteX / 4);
            yc = parent-> yc + (parent-> coteY / 4);
            coteX = parent-> coteX / 2;
            coteY = parent-> coteY / 2;
            *a = creerArbreQuat(xc, yc, coteX, coteY);
            // On insère le noeud.
            (*a)-> noeud = n;

            // On met à jour le pointeur du parent.
            parent-> ne = *a;

            return;
        }

        if( ((parent-> xc) > (n-> x)) && ((parent-> yc) <= (n-> y)) ){
            
            xc = parent-> xc - (parent-> coteX / 4);
            yc = parent-> yc + (parent-> coteY / 4);
            coteX = parent-> coteX / 2;
            coteY = parent-> coteY / 2;
            *a = creerArbreQuat(xc, yc, coteX, coteY);
            // On insère le noeud.
            (*a)-> noeud = n;

            // On met à jour le pointeur du parent.
            parent-> no = *a;

            return;
        }
    }

    // Si l'arbre est une feuille:
    if ( (*a)-> noeud != NULL ){
        Noeud* tmp = (*a)-> noeud;

        (*a)-> noeud = NULL;

        // On vérifie dans quel quadrant on va insérer le noeud déja présent.
        if (((*a)-> xc) > (tmp-> x) && ((*a)-> yc) > (tmp-> y)){
            insererNoeudArbre(tmp, &((*a)-> so), *a);
        }
        if (((*a)-> xc) <= (tmp-> x) && ((*a)-> yc) > (tmp-> y)){
            insererNoeudArbre(tmp, &((*a)-> se), *a);
        }
        if (((*a)-> xc) <= (tmp-> x) && ((*a)-> yc) <= (tmp-> y)){
            insererNoeudArbre(tmp, &((*a)-> ne), *a);
        }
        if (((*a)-> xc) > (tmp-> x) && ((*a)-> yc) <= (tmp-> y)){
            insererNoeudArbre(tmp, &((*a)-> no), *a);
        }

        // On vérifie dans quel quadrant on va insérer le nouveau noeud.
        if(((*a)-> xc) > (n-> x) && ((*a)-> yc) > (n-> y)){
            insererNoeudArbre(n, &((*a)-> so), *a);
            return;
        }
        if(((*a)-> xc) <= (n-> x) && ((*a)-> yc) > (n-> y)){
            insererNoeudArbre(n, &((*a)-> se), *a);
            return;
        }
        if(((*a)-> xc) <= (n-> x) && ((*a)-> yc) <= (n-> y)){
            insererNoeudArbre(n, &((*a)-> ne), *a);
            return;
        }
        if(((*a)-> xc) > (n-> x) && ((*a)-> yc) <= (n-> y)){
            insererNoeudArbre(n, &((*a)-> no), *a);
            return;
        }
    
    // Si on est dans une cellule interne:
    } else {

        // On vérifie dans quel quadrant on va insérer le nouveau noeud.
        if(((*a)-> xc) > (n-> x) && ((*a)-> yc) > (n-> y)){
            insererNoeudArbre(n, &((*a)-> so), *a);
            return;
        }
        if(((*a)-> xc) <= (n-> x) && ((*a)-> yc) > (n-> y)){
            insererNoeudArbre(n, &((*a)-> se), *a);
            return;
        }
        if(((*a)-> xc) <= (n-> x) && ((*a)-> yc) <= (n-> y)){
            insererNoeudArbre(n, &((*a)-> ne), *a);
            return;
        }
        if(((*a)-> xc) > (n-> x) && ((*a)-> yc) <= (n-> y)){
            insererNoeudArbre(n, &((*a)-> no), *a);
            return;
        }
    }
}



Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat** a, ArbreQuat* parent, double x, double y) {
    // Cas où l'arbre est vide
    if (*a == NULL) {
        Noeud * n = creeNoeudAQ(R, x, y);
        insererNoeudArbre(n, a, parent); // Insérer le nœud dans l'arbre
        return n;
    }
    // Cas où l'arbre est une feuille
    if ((*a)->noeud != NULL) {
        if (((*a)->noeud)->x ==x &&  ((*a)->noeud)->y ==y ){
            return (*a)->noeud;
        }
        else {
            Noeud * n = creeNoeudAQ(R, x, y);
            insererNoeudArbre(n, a, parent); // Insérer le nœud dans l'arbre
            return n;
        }
    }
    // Cas où l'arbre est une cellule interne
    // Déterminer dans quelle sous-cellule chercher le nœud du réseau de coordonnées (x, y)
    if (x < (*a)->xc) {
        if (y < (*a)->yc) {
            return rechercheCreeNoeudArbre(R, &(*a)->so, *a, x, y); // Cas sud-ouest
        } else {
            return rechercheCreeNoeudArbre(R, &(*a)->no, *a, x, y); // Cas nord-ouest
        }
    } else {
        if (y < (*a)->yc) {
            return rechercheCreeNoeudArbre(R, &(*a)->se, *a, x, y); // Cas sud-est
        } else {
            return rechercheCreeNoeudArbre(R, &(*a)->ne, *a, x, y); // Cas nord-est
        }
    }
    return NULL ;
} 







Reseau* reconstitueReseauArbre(Chaines *C) {
    Reseau *R = (Reseau*)malloc(sizeof(Reseau));
    R->nbNoeuds =0;
    R->gamma = C->gamma;
    R->noeuds = NULL;
    R->commodites = NULL;


    double xmin =-9999,ymin =-9999,xmax=9999,ymax=9999;
    chaineCoordMinMax(C, &xmin, &ymin, &xmax, &ymax);
    ArbreQuat * parent = creerArbreQuat ((xmin + xmax) / 2.0,(ymin + ymax) / 2.0,xmax - xmin,ymax - ymin);
    ArbreQuat * a=NULL ;

    CellChaine *chaine = C->chaines;
    while (chaine != NULL) {

        CellCommodite * nv_commodite = (CellCommodite*)malloc(sizeof(CellCommodite));
        CellPoint *point = chaine->points;
        Noeud *prec_noeud = NULL;
    
        while (point != NULL) {
            Noeud *noeud = rechercheCreeNoeudArbre(R,&a,parent, point->x, point->y);

            // si c'est pas le premier noeud
            if (prec_noeud != NULL) {
                    // Ajouter noeud à la liste des voisins de prec_noeud
                    construireLiaison ( noeud , prec_noeud);
                    //Ajouter prec_noeud à la liste des voisins de noeud pour la liaison bidirectionnelle
                    construireLiaison ( prec_noeud , noeud);
            }
            else {
                nv_commodite->extrA =noeud;
            }
            prec_noeud = noeud;
            point = point->suiv;
        }

        // Ajouter le reste des commodités de la chaîne à R->commodites 

        //nv_commodite->extrA = rechercheCreeNoeudArbre(R,&a, parent,chaine->points->x, chaine->points->y);
        nv_commodite->extrB = prec_noeud;
        nv_commodite->suiv= R->commodites;
        R->commodites =nv_commodite ;

        chaine=chaine->suiv;
    }
    liberer_ArbreQuat(parent);
    return R;
}


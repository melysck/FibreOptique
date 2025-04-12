#ifndef __ARBRE_QUAT_H__
#define __ARBRE_QUAT_H__
#include "Reseau.h"
/* Arbre quaternaire contenant les noeuds du reseau */
typedef struct arbreQuat{
    double xc, yc;          /* Coordonnees du centre de la cellule */	
    double coteX;           /* Longueur de la cellule */
    double coteY;           /* Hauteur de la cellule */
    Noeud* noeud;           /* Pointeur vers le noeud du reseau */
    struct arbreQuat *so;   /* Sous-arbre sud-ouest, pour x < xc et y < yc */
    struct arbreQuat *se;   /* Sous-arbre sud-est, pour x >= xc et y < yc */
    struct arbreQuat *no;   /* Sous-arbre nord-ouest, pour x < xc et y >= yc */
    struct arbreQuat *ne;   /* Sous-arbre nord-est, pour x >= xc et y >= yc */
} ArbreQuat;



/* ~~~~~~ Fonctions ajoutées pour un code modulaire / libération  ~~~~~~ */

Noeud * creeNoeudAQ (Reseau *R ,double x , double y );
void liberer_ArbreQuat(ArbreQuat* aq);


/* ~~~~~~ Fonctions principales ~~~~~~ */

ArbreQuat* creerArbreQuat(double xc, double yc, double coteX,
double coteY);
void chaineCoordMinMax(Chaines* C, double* xmin, double*
ymin, double* xmax, double* ymax);
Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat** a, ArbreQuat* parent, double x, double y) ;
Reseau* reconstitueReseauArbre(Chaines *C) ;
void insererNoeudArbre(Noeud* n, ArbreQuat** a, ArbreQuat* parent);



#endif

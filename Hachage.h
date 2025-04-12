#ifndef __HACHAGE_H__
#define __HACHAGE_H__	
#include<stdio.h>
#include "Chaine.h"
#include "Liste.h"

typedef struct{
  int nbElement; //pas necessaire ici
  int tailleMax;
  CellNoeud** T;
} TableHachage ;


/* ~~~~~~ Fonctions ajoutées pour un code modulaire / libération  ~~~~~~ */
void libererTableHachage(TableHachage *H);
Noeud * creeNoeud (Reseau * R , double x , double y );



/* ~~~~~~ Fonctions principales ~~~~~~ */

int fonctionHachage(int M, double k);
double Clef(double x, double y);
Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage*
H, double x, double y);
TableHachage* initialiseTablehachage(int m);
Reseau* reconstitueReseauHachage(Chaines* C, int M);



#endif	


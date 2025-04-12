#ifndef __GRAPHE_H__
#define __GRAPHE_H__

#include <stdlib.h>
#include <stdio.h>
#include "Reseau.h"
#include "Chaine.h"

typedef struct {
    int u, v; /* Numeros des sommets extremite */
} Arete;

typedef struct cellule_arete {
    Arete* a; /* pointeur sur l’arete */
    struct cellule_arete* suiv;
} Cellule_arete;

typedef struct {
    int num; /* Numero du sommet (le meme que dans T_som) */
    double x, y;
    Cellule_arete* L_voisin; /* Liste chainee des voisins */
} Sommet;

typedef struct {
    int e1, e2; /* Les deux extremites de la commodite */
} Commod;

typedef struct {
    int nbsom; /* Nombre de sommets */
    Sommet** T_som; /* Tableau de pointeurs sur sommets */
    int gamma;
    int nbcommod; /* Nombre de commodites */
    Commod* T_commod; /* Tableau des commodites */
} Graphe;



// Structure d'un élément de la liste chaînée
typedef struct NoeudListe {
    int valeur;
    struct NoeudListe* suivant;
} NoeudListe;

// Structure de la liste chaînée
typedef struct {
   NoeudListe* tete;
} ListeChaine;

//Fonctions pour la création d'un graphe 
Sommet* creer_sommet(int num, double x, double y);
Cellule_arete* creer_L_voisins(int u, int v);
void ajouter_voisins(Sommet* s, Arete* a);
Graphe* creerGraphe(Reseau* r) ;


// Fonction d'affichage 
void afficher_commod(Commod commod);
void afficher_arete(Arete* a);
void afficher_sommet(Sommet* s);
void afficher_L_voisins(Sommet* s);
void afficher_graphe(Graphe* G);
void afficher_graphe(Graphe* G);
void afficherListeChainee(ListeChaine* liste) ;


//Fonctions pour gérer les listes 

void ajouterEnTete(ListeChaine* liste, int valeur) ;
void libererListe(ListeChaine* liste) ;
void libererGraphe(Graphe* g) ;




int plusPetitNombreAretes(Graphe* G, int u, int v) ;

ListeChaine* cheminEntreDeuxSommets(Graphe* G, int u, int v) ;

int reorganiseReseau(Reseau* r) ;

#endif

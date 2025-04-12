#include <stdio.h>
#include <stdlib.h>
#include "Reseau.h"
#include "Chaine.h"
#include "Graphe.h"
#include "Struct_File.h"



Sommet* creer_sommet(int num, double x, double y){
    Sommet* s = (Sommet*)malloc(sizeof(Sommet));
    if(s == NULL){
        printf("Erreur : sommet non alloué\n");
        return NULL;
    }
    s->num = num;
    s->x = x;
    s->y = y;
    s->L_voisin = NULL;
    return s;

}
Cellule_arete* creer_L_voisins(int u, int v){
    Arete* a = (Arete*)malloc(sizeof(Arete));
    a->u = u;
    a->v = v;
    Cellule_arete* ca = (Cellule_arete*)malloc(sizeof(Cellule_arete));
    ca->a = a;
    ca->suiv = NULL;
    return ca;
}

void ajouter_voisins(Sommet* s, Arete* a){
    Cellule_arete* tmp =(Cellule_arete*)malloc(sizeof(Cellule_arete));
    tmp->a = a;
    tmp->suiv = s->L_voisin ;
    s->L_voisin = tmp;
}

Graphe* creerGraphe(Reseau* R){
    Graphe* G = (Graphe*) malloc(sizeof(Graphe));
    if (G == NULL){
        printf("Erreur : Graphe non alloué\n");
        return NULL ;
    }
    G->nbsom = R->nbNoeuds;
    G->gamma = R->gamma;
    G->nbcommod = nbCommodites(R);
    G->T_som = (Sommet**)malloc(sizeof(Sommet*)*G->nbsom);
    //on cree les sommets et on les ajoutes dans le tableau 
    CellNoeud* cn = R->noeuds;
    for(int i=0; i<G->nbsom; i++){
        if(cn == NULL) break;
        Noeud* n = cn->nd;
       if(n->num <= G->nbsom){
            G->T_som[n->num]=creer_sommet(n->num,n->x,n->y);
       }
        cn = cn->suiv;
    }
    // on ajoute les voisins pour chaque sommet
    cn = R->noeuds;
    for(int i=0; i<G->nbsom; i++){
        if(cn == NULL){
            printf("Liste de noeuds est vide \n");
            break;
        } 
        Noeud* n_cour = cn->nd;
        CellNoeud* L_voisin = n_cour->voisins;
        while(L_voisin){
            Noeud* nd_voisin = L_voisin->nd;
            if(nd_voisin->num < n_cour->num){
                Arete* a = (Arete*)malloc(sizeof(Arete));
                a->u = n_cour->num;
                a->v = nd_voisin->num;
            
                ajouter_voisins((G->T_som[n_cour->num]),a);
                ajouter_voisins((G->T_som[nd_voisin->num]),a);
            }
            L_voisin = L_voisin->suiv;
        }
        cn=cn->suiv;
    }

    CellCommodite* cc = R->commodites;
    G->T_commod = (Commod*)malloc(sizeof(Commod)*G->nbcommod);
    for(int i=0; i<G->nbcommod; i++){
        if(cc == NULL) break;
        G->T_commod[i].e1 = cc->extrA->num;
        G->T_commod[i].e2 = cc->extrB->num;
        cc = cc->suiv;
    }

    return G ;
    
}
void afficher_commod(Commod commod){
    printf("e1 = %d, e2 = %d \n",commod.e1,commod.e2);
}
void afficher_arete(Arete* a){
    printf("u = %d, v = %d\n",a->u,a->v);
}
void afficher_sommet(Sommet* s){
    printf("num = %d, x = %.2lf, y = %.2lf\n",s->num, s->x, s->y);
}
void afficher_L_voisins(Sommet* s){// afficher les voisins d'un sommet
    Cellule_arete* voisins = s->L_voisin;
    if(voisins == NULL){
        printf("L_voisins est vide \n");
    }
    while(voisins){
        afficher_arete(voisins->a);
        voisins = voisins->suiv;
    }
}


void afficher_graphe(Graphe* G){
    printf("nb_sommets : %d , gamma : %d\n",G->nbsom,G->gamma);
    printf("\n Les Sommets :\n");
    if (G->T_som == NULL) {
        printf("Tableau de sommets non initialisé.\n");
        return;
    }
    for (int i=1; i<= G->nbsom; i++){
        if (G->T_som[i] == NULL) {
            printf("Sommet %d non initialisé.\n", i);
            continue;  
        }
        printf("%d\n",i);
        Sommet* s = G->T_som[i];
        printf("num = %d, x = %.2lf, y = %.2lf pos = %d \n",s->num, s->x, s->y, i);
    }
    printf("\nLes commod :\n");
    printf("nb de commod :%d\n",G->nbcommod);
    if (G->T_commod == NULL) {
        printf("Tableau de commodités non initialisé.\n");
        return;
    }
    Commod* T_commod = G->T_commod;
    for(int i=0; i<G->nbcommod; i++){
        afficher_commod(T_commod[i]);
    }
    printf("\n Les Sommets et leurs voisins\n");
    for (int i=1; i<= G->nbsom; i++){
        if (G->T_som[i] == NULL) {
            printf("Sommet %d non initialisé.\n", i);
            continue; 
        }
        Sommet* s = G->T_som[i];
        printf("Le sommet %d\n",s->num);
        afficher_L_voisins(s);
    }
    printf("\n");
}


// Fonction pour ajouter un élément en tête de la liste
void ajouterEnTete(ListeChaine* liste, int valeur) {
    NoeudListe* nouveauNoeud = (NoeudListe*)malloc(sizeof(NoeudListe));
    nouveauNoeud->valeur = valeur;
    nouveauNoeud->suivant = liste->tete;
    liste->tete = nouveauNoeud;
}

// Fonction pour libérer la mémoire occupée par la liste
void libererListe(ListeChaine* liste) {
    NoeudListe* actuel = liste->tete;
    while (actuel != NULL) {
        NoeudListe* suivant = actuel->suivant;
        free(actuel);
        actuel = suivant;
    }
    liste->tete = NULL;
}
// Fonction pour afficher les valeurs de la liste chaînée
void afficherListeChainee(ListeChaine* liste) {
    printf("Liste d'entiers (chemin) : ");
    NoeudListe * l =liste->tete;
    while (l != NULL) {
        printf("%d ", l->valeur );
        l = l->suivant;
    }
    printf("\n");
}


int trouve_voisin(Cellule_arete* C,int num){
    int u=C->a->u;
    int v=C->a->v;
    if (u==num){ 
        return v;
    }
    return u;
}


// Parcours en largeur pour trouver le plus court chemin

int plusPetitNombreAretes(Graphe* G, int u, int v) {

    int* visite = (int*)malloc(G->nbsom * sizeof(int));
    
    //int* pere = (int*)malloc(G->nbsom * sizeof(int));

    int * D =(int*)malloc(G->nbsom * sizeof(int));

    //création d'une file
    S_file * f =malloc(sizeof(S_file));
    Init_file(f);

    //initialisation du tableau père et distance 
    for (int i = 0; i <= G->nbsom; i++) {
        visite[i] = 0;
        //pere[i] = -1;
        D[i]=999;
    }
    D[u]=0;
    visite[u] = 1;
    enfile(f, u);

    //pere[u]=u;//ici il faut inialisé pere[u]
    while (!estFileVide(f)) {
        int noeud = defile(f);
        //if (noeud == v) {
         //   break; // On a trouvé le sommet destination
       // }
        // Parcours des voisins
        Cellule_arete* cour = G->T_som[noeud]->L_voisin;
        while (cour != NULL) {

            int voisin = trouve_voisin (cour , noeud);
            if (visite[voisin] == 0) {
                D[voisin]=D[noeud]+1;
                visite[voisin] = 1;
               // printf(" le pere de %d est %d \n",voisin, noeud);

                //pere[voisin] = noeud;
                enfile(f, voisin);
            }
            cour = cour->suiv;
        }
    }



    // Calcul du nombre d'arêtes du plus court chemin
    // int nbAretes = 0;
    // int noeudCourant = v;
    // printf(" v=%d \n", noeudCourant);
    // while (noeudCourant != -1 ) {
    //     int p = pere[noeudCourant];
    //     printf(" le pere de %d est %d \n",noeudCourant,p);
    //     nbAretes++;
    //    if  (pere[noeudCourant] == u) break ;

    //     noeudCourant = p;
    // }
    
    int min_nbArete=D[v];
    // Libération de la mémoire
    free(visite);
    //free(pere);
    free(D);
    return min_nbArete;
}



ListeChaine* cheminEntreDeuxSommets(Graphe* G, int u, int v) {
    int* visite = (int*)malloc(G->nbsom * sizeof(int));
    int* pere = (int*)malloc(G->nbsom * sizeof(int));

    S_file* f = malloc(sizeof(S_file));
    Init_file(f);

    // Initialisation du tableau père
    for (int i = 0; i < G->nbsom; i++) {
        visite[i] = 0;
        pere[i] = -1; 
    }
    visite[u] = 1;
    enfile(f, u);

    while (!estFileVide(f)) {
        int noeud = defile(f);
        // Parcours des voisins
        Cellule_arete* cour = G->T_som[noeud]->L_voisin;
        while (cour != NULL) {
            int voisin = trouve_voisin (cour , noeud);
            if (!visite[voisin]) {

                visite[voisin] = 1;
                pere[voisin] = noeud;
                enfile(f, voisin);
            }
            cour = cour->suiv;
        }
    }

    //on peut trouver le plus court chemin de u à un sommet quelconque
    // Reconstruction du chemin de u à v
    ListeChaine* chemin = (ListeChaine*)malloc(sizeof(ListeChaine));
    chemin->tete = NULL;
    int sommetCourant = v;
    while (sommetCourant != -1) {
        ajouterEnTete(chemin, sommetCourant);
        sommetCourant = pere[sommetCourant];
    }

    // Libération de la mémoire
    free(visite);
    free(pere);
    return chemin;
}




void libererGraphe(Graphe* g) {
    if (g == NULL) return;

    // Libérer la mémoire allouée pour les sommets et leurs listes de voisins
    for (int i = 1; i <= g->nbsom; i++) {
        Cellule_arete* current = g->T_som[i]->L_voisin;
        while (current != NULL) {
            Cellule_arete* next = current->suiv;
            free(current->a); // Libérer l'arete pointée par la cellule
            free(current); // Libérer la cellule
            current = next;
        }
        free(g->T_som[i]); // Libérer le sommet
    }
    free(g->T_som); // Libérer le tableau de pointeurs de sommets

    // Libérer la mémoire allouée pour les commodités
    free(g->T_commod);

    // Libérer la mémoire allouée pour la structure du graphe
    free(g);
}


int reorganiseReseau(Reseau* r) {
    // Création du graphe correspondant au réseau
    Graphe* G = creerGraphe(r);

    // Calcul de la plus courte chaîne pour chaque commodité
    for (int i = 0; i < G->nbcommod; i++) {
        Commod commodite = G->T_commod[i];
        int nbAretes = plusPetitNombreAretes(G, commodite.e1, commodite.e2);
        // printf("Plus courte chaîne pour la commodité %d d'extrémités (%d,%d): %d arêtes\n", i + 1, commodite.e1, commodite.e2, nbAretes);
    }

    // Création d'une matrice pour compter le nombre de chaînes passant par chaque arête + initialisation du compteur à zéro
    int** compteurAretes=(int**)malloc(sizeof(int*)*(G->nbsom+1));

    for(int i=0;i<G->nbsom+1;i++)
    {
        compteurAretes[i]=(int*)malloc(sizeof(int)*(G->nbsom+1));
        for (int j=0;j<G->nbsom+1;j++)
        { 
            compteurAretes[i][j]=0;
        }
    }
    // Remplir la matrice comptant le nombre de chaînes passant par chaque arête
    for (int i = 0; i < G->nbcommod; i++) {
        Commod commodite = G->T_commod[i];
        int nbAretes = plusPetitNombreAretes(G, commodite.e1, commodite.e2);
        ListeChaine *chemin = cheminEntreDeuxSommets(G, commodite.e1, commodite.e2);
        if (chemin->tete == NULL) break;

        NoeudListe *c = chemin->tete;
        for (int j = 0; j < nbAretes; j++) {
            while (c != NULL && c->suivant != NULL) {
                int u = c->valeur;
                int v = c->suivant->valeur;
                compteurAretes[u][v]++;
                c = c->suivant;
            }
        }
        libererListe(chemin);
    }

    // Vérification si le nombre de chaînes passant par chaque arête est inférieur ou égal à gamma
    int estReorganise = 0;
    for (int i = 0; i < G->nbsom+1; i++) {
        for (int j = 0; j < G->nbsom+1; j++) {
            if (compteurAretes[i][j] > G->gamma) {
                estReorganise = 1;
                break;
            }
        }
        if (estReorganise) {
            break;
        }
    }

    // Libération de la mémoire allouée dynamiquement
    for (int i = 0; i < G->nbsom+1; i++) {
        free(compteurAretes[i]);
    }
    free(compteurAretes);

    return estReorganise;
}

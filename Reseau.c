#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Chaine.h"
#include "SVGwriter.h"
#include "Reseau.h"
#include <assert.h>



Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y){
    
    CellNoeud * l_noeud = R->noeuds;
    while ( l_noeud){
        Noeud * n = l_noeud->nd ;
        if ( n && n->x==x && n->y==y ){
            return n ;
        }
        l_noeud= l_noeud->suiv;
    }
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
    return n_nv ;
}


Reseau* reconstitueReseauListe(Chaines *C) {
    Reseau *R = (Reseau*)malloc(sizeof(Reseau));
    R->nbNoeuds =0;
    R->gamma = C->gamma;
    R->noeuds = NULL;
    R->commodites = NULL;
    CellChaine *chaine = C->chaines;
    while (chaine != NULL) {
        CellPoint *point = chaine->points;
        Noeud *prec_noeud = NULL;
    
        while (point != NULL) {
            Noeud *noeud = rechercheCreeNoeudListe(R, point->x, point->y);
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
        nv_commodite->extrA = rechercheCreeNoeudListe(R,chaine->points->x, chaine->points->y);
        nv_commodite->extrB = prec_noeud;
        nv_commodite->suiv= R->commodites;
        R->commodites =nv_commodite ;
        chaine=chaine->suiv;
    }

    return R;
}

// Fonction pour construire une liaison entre deux nœuds
void construireLiaison(Noeud *n1, Noeud *n2) {
    // Vérifier si la liaison n'existe pas déjà
    int liaisonExiste = 0;
    CellNoeud *voisin = n1->voisins;
    while (voisin != NULL) {
        if (voisin->nd == n2) {
            liaisonExiste = 1;
            break;
        }
        voisin = voisin->suiv;
    }
    // Si la liaison n'existe pas déjà, l'ajouter dans les deux sens
    if (!liaisonExiste) {
        ajouterVoisin(n1, n2);
        ajouterVoisin(n2, n1);
    }
}

// Fonction pour ajouter un nœud à la liste des voisins d'un autre nœud
void ajouterVoisin(Noeud *n, Noeud *voisin) {
    CellNoeud *nv_voisin = (CellNoeud*)malloc(sizeof(CellNoeud));
    nv_voisin->nd = voisin;
    nv_voisin->suiv = n->voisins;
    n->voisins = nv_voisin;
}



// Fonction pour compter le nombre de commodités dans un réseau
int nbCommodites(Reseau *R) {
    int count = 0;
    CellCommodite *commodite = R->commodites;
    while (commodite != NULL) {
        count++;
        commodite = commodite->suiv;
    }
    return count;
}



int nbLiaisons(Reseau *R){
    CellNoeud *noeuds = R->noeuds;
    CellNoeud *voisins;
    int cpt = 0;

    while(noeuds){
        if ( noeuds->nd){
            voisins = noeuds->nd->voisins;
            while(voisins){

                if(noeuds->nd->num < voisins->nd->num){
                    cpt++;
                }

                voisins = voisins->suiv;
            }
        }
        noeuds = noeuds->suiv;
     }
    return cpt;                                                                                                                                                                                                                                                                                                                          
}



void ecrireReseau(Reseau *R, FILE *f){
    if (R==NULL)return ;
    fprintf(f , "NbNoeuds : %d\nNbLiaisons : %d\nNbCommodites : %d\nGamma : %d\n\n", R->nbNoeuds,nbLiaisons(R),nbCommodites(R),R->gamma);
    CellNoeud * noeud = R->noeuds;
    while (noeud){
        Noeud * n = noeud->nd ;
        if ( n){
            fprintf(f, "v %d %lf %lf\n", n->num , n->x , n->y);
        }
        noeud=noeud->suiv ;
    }
    fprintf(f, "\n");
    noeud = R->noeuds;
    while (noeud){
        Noeud * n = noeud->nd ;
        if ( n ){
            CellNoeud * voisins= n->voisins;
            while (voisins){
                Noeud * n_voisin = voisins->nd;
                if ( n_voisin && n->num < n_voisin->num ){
                    fprintf(f, "l %d %d\n", n->num ,n_voisin->num );
                }
                voisins= voisins->suiv;
            }
        }
        noeud = noeud ->suiv;
    }
    fprintf(f,"\n");
    CellCommodite * commodite = R->commodites;
    while (commodite){
        if (commodite->extrA && commodite->extrB){
            fprintf(f , "k %d %d\n",commodite->extrA->num , commodite->extrB->num);
        }
        commodite =commodite->suiv ;
    }
}

void detruireReseau(Reseau* res){
    /*
    Libère toute la mémoire associée au réseau res.
    */
    if(res == NULL){
        return;
    }

    // Libération de la mémoire associée aux commodités:
    CellCommodite* tmp_com;
    while(res-> commodites){
        tmp_com = (res-> commodites);
        (res-> commodites) = (res-> commodites)-> suiv;
        free(tmp_com);
    }

    // Libération de la mémoire associée aux noeuds et à leurs voisins:

    CellNoeud* tmp;
    CellNoeud* tmp_voisin;

    while(res-> noeuds){

        tmp = (res-> noeuds);
        tmp_voisin = (tmp-> nd)-> voisins;
        
        while(tmp_voisin){
            (tmp-> nd-> voisins) = (tmp-> nd-> voisins)-> suiv;
            free(tmp_voisin);
            tmp_voisin = tmp-> nd-> voisins;
        }

        (res-> noeuds) = (res-> noeuds)-> suiv;

        free(tmp-> nd);
        free(tmp);
    }

    free(res);

}
void afficheReseauSVG(Reseau *R, char* nomInstance){
    CellNoeud *courN,*courv;
    SVGwriter svg;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;

    courN=R->noeuds;
    while (courN!=NULL){
        if (maxx<courN->nd->x) maxx=courN->nd->x;
        if (maxy<courN->nd->y) maxy=courN->nd->y;
        if (minx>courN->nd->x) minx=courN->nd->x;
        if (miny>courN->nd->y) miny=courN->nd->y;
        courN=courN->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    courN=R->noeuds;
    while (courN!=NULL){
        SVGpoint(&svg,500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
        courv=courN->nd->voisins;
        while (courv!=NULL){
            if (courv->nd->num<courN->nd->num)
                SVGline(&svg,500*(courv->nd->x-minx)/(maxx-minx),500*(courv->nd->y-miny)/(maxy-miny),500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
            courv=courv->suiv;
        }
        courN=courN->suiv;
    }
    SVGfinalize(&svg);
}


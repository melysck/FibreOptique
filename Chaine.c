#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "Chaine.h"
#include "SVGwriter.h"
#include "Liste.h"




/* ~~~~~~ Fonctions principales ~~~~~~ */


Chaines* lectureChaines(FILE *f) {
    Chaines* result = (Chaines*)malloc(sizeof(Chaines));
    if (result == NULL) {
        printf("Erreur d'allocation de mémoire.\n");
        exit(EXIT_FAILURE);
    }

    fscanf(f, "NbChain : %d\n", &(result->nbChaines));
    fscanf(f, "Gamma : %d\n", &(result->gamma));

    result->chaines = NULL; // Initialisation de la liste de chaines

    // Lecture de chaque chaine
    for (int i = 0; i < result->nbChaines; i++) {
        CellChaine* nouvelleChaine = (CellChaine*)malloc(sizeof(CellChaine));
        if (nouvelleChaine == NULL) {
            printf("Erreur d'allocation de mémoire.\n");
            exit(EXIT_FAILURE);
        }
        nouvelleChaine->suiv = result->chaines; // Ajout en tête de la liste
        result->chaines = nouvelleChaine;
        int nbPoints;

        fscanf(f, "%d %d", &(nouvelleChaine->numero), &nbPoints);

        nouvelleChaine->points = NULL; // Initialisation de la liste de points pour cette chaine

        // Lecture de chaque point
        for (int j = 0; j < nbPoints; j++) {
            CellPoint* nouveauPoint = (CellPoint*)malloc(sizeof(CellPoint));
            if (nouveauPoint == NULL) {
                printf("Erreur d'allocation de mémoire.\n");
                exit(EXIT_FAILURE);
            }

            fscanf(f, "%lf %lf", &(nouveauPoint->x), &(nouveauPoint->y));
            nouveauPoint->suiv = nouvelleChaine->points; // Ajout en tête de la liste
            nouvelleChaine->points = nouveauPoint;
        }
    }

    return result;
}


int comptePoints(CellPoint *points) {
    int count = 0;
    CellPoint *current = points;
    while (current != NULL) {
        count++;
        current = current->suiv;
    }
    return count;
}

void ecrireChaines(Chaines *C, FILE *f) {

    if (C == NULL || f == NULL) {
        fprintf(stderr, "Erreur : pointeur Chaines ou pointeur FILE invalide.\n");
        return;
    }
    fprintf(f, "NbChain: %d\nGamma: %d\n", C->nbChaines, C->gamma);
    
    CellChaine *chaine = C->chaines;
    while (chaine != NULL) {
        fprintf(f, "%d %d", chaine->numero, comptePoints(chaine->points));
        printf("boucle ");
        CellPoint *point = chaine->points;
        while (point != NULL) {
            fprintf(f, " %.2lf %.2lf", point->x, point->y);
            point = point->suiv;
        }
        
        fprintf(f, "\n"); 
        chaine = chaine->suiv;
    }
} 


double distance(CellPoint *p1, CellPoint *p2) {
    return sqrt(pow(p2->x - p1->x, 2) + pow(p2->y - p1->y, 2));
}

double longueurChaine(CellChaine *c) {
    double longueur = 0.0;
    CellPoint *point1 = c->points;
    CellPoint *point2 = NULL;
    while (point1 != NULL && point1->suiv != NULL) {
        point2 = point1;
        point1=point1->suiv;
        longueur += distance(point1, point2);
    }

    return longueur;
}

double longueurTotale(Chaines *C) {
    double longueurTotale = 0.0;
    CellChaine *chaine = C->chaines;

    while (chaine != NULL) {
        longueurTotale += longueurChaine(chaine);
        chaine = chaine->suiv;
    }

    return longueurTotale;
}
int comptePointsTotal(Chaines *C) {
    int totalPoints = 0;
    CellChaine *chaine = C->chaines;
    
        while (chaine != NULL) {
        totalPoints += comptePoints(chaine->points);
        chaine = chaine->suiv;
    }
    
    return totalPoints;
}




void afficheChainesSVG(Chaines *C, char* nomInstance){
    //int i;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;
    CellChaine *ccour;
    CellPoint *pcour;
    double precx,precy;
    SVGwriter svg;
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        while (pcour!=NULL){
            if (maxx<pcour->x) maxx=pcour->x;
            if (maxy<pcour->y) maxy=pcour->y;
            if (minx>pcour->x) minx=pcour->x;
            if (miny>pcour->y) miny=pcour->y;  
            pcour=pcour->suiv;
        }
    ccour=ccour->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        SVGlineRandColor(&svg);
        SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny)); 
        precx=pcour->x;
        precy=pcour->y;  
        pcour=pcour->suiv;
        while (pcour!=NULL){
            SVGline(&svg,500*(precx-minx)/(maxx-minx),500*(precy-miny)/(maxy-miny),500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            precx=pcour->x;
            precy=pcour->y;    
            pcour=pcour->suiv;
        }
        ccour=ccour->suiv;
    }
    SVGfinalize(&svg);
}

/* ~~~~~~ Fonctions ajoutées pour un code modulaire / libération  ~~~~~~ */

void libererChaines(Chaines *C) {
    if (C == NULL) {
        return;
    }
    
    // Libération de la mémoire pour chaque chaîne
    CellChaine *chaine = C->chaines;
    while (chaine != NULL) {
        // Libération de la mémoire pour chaque point dans la chaîne
        CellPoint *point = chaine->points;
        while (point != NULL) {
            CellPoint *temp = point;
            point = point->suiv;
            free(temp); // Libération de la mémoire pour le point actuel
        }
        
        CellChaine *temp_chaine = chaine;
        chaine = chaine->suiv;
        free(temp_chaine); // Libération de la mémoire pour la chaîne actuelle
    }
    
    free(C);
}


/* Fonction pour générer un nombre aléatoire entre min et max */
double random_double(double min, double max) {
    return min + (rand() / (RAND_MAX / (max - min)));
}


/* Fonction pour créer une chaîne de points aléatoires */
Chaines* generationAleatoire(int nbChaines, int nbPointsChaine, int xmax, int ymax) {
    Chaines* chaines = (Chaines*)malloc(sizeof(Chaines));
    if (chaines == NULL) {
        fprintf(stderr,"Erreur lors de l'allocation de mémoire pour les chaînes");
        return NULL;
    }
    
    chaines->gamma = 3; 
    chaines->nbChaines = nbChaines;
    chaines->chaines = NULL;

    srand(time(NULL)); // Initialisation de la graine pour la génération aléatoire

    for (int i = 0; i < nbChaines; i++) {

        CellChaine* nouvelleChaine = (CellChaine*)malloc(sizeof(CellChaine));
        if (nouvelleChaine == NULL) {
            fprintf(stderr,"Erreur lors de l'allocation de mémoire pour les chaînes");
            libererChaines(chaines); // Libérer la mémoire allouée jusqu'à présent
            return NULL;
        }
        
        nouvelleChaine->numero = i;
        nouvelleChaine->points = NULL;

        for (int j = 0; j < nbPointsChaine; j++) {
            CellPoint* nouveauPoint = (CellPoint*)malloc(sizeof(CellPoint));
            
            if (nouveauPoint == NULL) {
                perror("Erreur lors de l'allocation de mémoire pour le nouveau point");
                libererChaines(chaines); // Libérer la mémoire allouée jusqu'à présent
                return NULL;
            }
            
            nouveauPoint->x = random_double(0, xmax);
            nouveauPoint->y = random_double(0, ymax);
            nouveauPoint->suiv = nouvelleChaine->points;
            nouvelleChaine->points = nouveauPoint;
        }

        nouvelleChaine->suiv = chaines->chaines;
        chaines->chaines = nouvelleChaine;
    }

    return chaines;
}

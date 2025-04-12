#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "Reseau.h"
#include "Hachage.h"
#include "ArbreQuat.h"
#include "Chaine.h"

/* ~~~~~~ Fonctions auxiliaires ~~~~~~ */
double time_cpu(int start, int end){
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

double time_hachage_diff_sizes(Chaines* C) {
    /*
    Calcule le temps CPU pour reconstruire le réseau pour différentes tailles de table de hachage.
    */

    double start, end, temps_moy;
    int size = 10;
    int nb_tests = 0;
    double temps_tot = 0.0; // Utilisation d'un double pour le temps total

    while (size <= 50) {
        start = clock();
        Reseau* R = reconstitueReseauHachage(C, size);
        end = clock();
        temps_tot += time_cpu(start, end);
        detruireReseau(R); 
        nb_tests++;
        size += 10;
    }

    temps_moy = temps_tot / nb_tests; // Pas besoin de multiplier par 1.0 pour obtenir un résultat en double

    return temps_moy;
}

//fonction pour calculer la moyenne des temps 
double moy_struct(Chaines *c, int Meth, int M) {
    int nb = 0;
    double temp = 0.0;
    Reseau* R = NULL;
    while (nb < 3) {
        switch (Meth) {
        case 0: {
            clock_t start = clock();
            R = reconstitueReseauListe(c);
            clock_t end = clock();
            temp += time_cpu(start, end);
            detruireReseau(R);
            break;
        }
        case 1: {
            clock_t start = clock();
            R = reconstitueReseauHachage(c, M);
            clock_t end = clock();
            temp += time_cpu(start, end);
            detruireReseau(R);
            break;
        }
        case 2: {
            clock_t start = clock();
            R = reconstitueReseauArbre(c);
            clock_t end = clock();
            temp += time_cpu(start, end);
            detruireReseau(R);
            break;
        }
        default:
            fprintf(stderr, "Méthode non reconnue.\n");
            break;
        }
        nb++;
    }
    return temp / ((double)nb);
}



/* ~~~~~~ Fonctions principales ~~~~~~ */


void time_diff_structures(){

    Chaines* C = NULL;
    // Reseau* R = NULL;
    // double start, end;
    char str[51];

    FILE* w = fopen("timeCal.txt", "w");
    if(w == NULL){
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'ouverture du fichier.\n", __FILE__, __LINE__);
        return;
    }

    
    for(int i = 1; i <= 3; i++){
        sprintf(str, "instance%d.cha", i);
        FILE* f = fopen(str, "r");
        if(f == NULL){
            fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'ouverture du fichier.\n", __FILE__, __LINE__);
            return;
        }
        C = lectureChaines(f);
        fclose(f);

        fprintf(w, "Instance %d:\t", i);

        // Calcule du temps cpu pour reconstruire le reseau, pour une liste chainée.
        // start = clock();
        // R = reconstitueReseauListe(C);
        // end = clock();
        // detruireReseau(R);
        
        fprintf(w, "%f\t", moy_struct(C , 0 , 0));//troixième argument pas utilisé

        // Calcule du temps cpu pour reconstruire le reseau, pour une table de hachage.
        // start = clock();
        // R = reconstitueReseauHachage(C, 10);
        // end = clock();
        // detruireReseau(R);
        fprintf(w, "%f\t", moy_struct(C , 1, 10));

        // Calcule du temps cpu pour reconstruire le reseau, pour un arbre quaternaire.
        // start = clock();
        // R = reconstitueReseauArbre(C);
        // end = clock();
        // detruireReseau(R);
        fprintf(w, "%f\t", moy_struct(C ,2, 2));

        fprintf(w, "\n");
        libererChaines(C);

    }

    fclose(w);
}

double time_HT_file(int M) {
    FILE* w = fopen("time_HT.txt", "w");
    if(w== NULL) {
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'ouverture du fichier.\n", __FILE__, __LINE__);
        exit(1);
    }

    Chaines* C = NULL;
    int nbPointsChaine = 100;
    int xmax = 5000;
    int ymax = 5000;
    int nbChaines = 500;
    double temp;
    while(nbChaines <= 5000) {
        printf("nbChaines: %d\n", nbChaines);
        C = generationAleatoire(nbChaines, nbPointsChaine, xmax, ymax);

        // TABLE DE HACHAGE

        temp=moy_struct(C , 1, M);

        fprintf(w, "%d\t%f\n", nbChaines*100, temp);

        nbChaines += 500;
    }
    libererChaines(C);

    fclose(w);
    return temp;

}




void comparaison_taille(){
   FILE * f = fopen ( "comparaison_taille_M", "w");
   if (f==NULL)exit (1);
   for (int i = 1000; i<=4000 ; i=i+1000){
       fprintf(f, "%d\t%f \n",i,time_HT_file(i) );
   }
   fclose(f);
}


void time_LC_file(){

    FILE* w = fopen("time_LC.txt", "w");
    if(w == NULL){
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'ouverture du fichier.\n", __FILE__, __LINE__);
        return;
    }

    Chaines* C = NULL;
    Reseau* R = NULL;

    double start, end;

    int nbPointsChaine = 100;
    int xmax = 5000;
    int ymax = 5000;
    int nbChaines = 50;

    while(nbChaines <= 500){
        printf("nbChaines: %d\n", nbChaines);
        C = generationAleatoire(nbChaines, nbPointsChaine, xmax, ymax);

        start = clock();
        R = reconstitueReseauListe(C);
        end = clock();

        fprintf(w, "%d\t%f\n", nbChaines *100 , time_cpu(start,end));

        detruireReseau(R);
        nbChaines += 50;
    }
    libererChaines(C);

    fclose(w);
}


void time_AQ_file() {
    FILE* w = fopen("time_AQ.txt", "w");
    if(w == NULL) {
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'ouverture du fichier.\n", __FILE__, __LINE__);
        return;
    }
    Chaines* C = NULL;
    // Reseau* R = NULL;
    // double start, end;
    int nbPointsChaine = 100;
    int xmax = 5000;
    int ymax = 5000;
    int nbChaines = 500;

    while(nbChaines <= 5000) {
        printf("nbChaines: %d\n", nbChaines);
        C = generationAleatoire(nbChaines, nbPointsChaine, xmax, ymax);

        //ARBRE QUATERNAIRE
        // start = clock();
        // R = reconstitueReseauArbre(C);
        // end = clock();
        fprintf(w, "%d\t%f\n", nbChaines*100 ,moy_struct(C , 2, 2));

        nbChaines += 500;
    }
    libererChaines(C);

    fclose(w);

}

int main() {

    time_HT_file(2000);
    // comparaison_taille();
    // time_AQ_file();

    // time_LC_file();

    // time_diff_structures();
    return 0;
}
//plus la taille de la table de hachage est grande plus les temps d'executions diminuent 

// # Script gnuplot pour tracer les courbes
/*
set terminal png \
 set output 'temps_calcul_LC_HT_AQ.png'\
 set title "Temps de reconstition du reseau en fonction du nombre de chaines LC HT AQ"\
 set xlabel "Nombre de chaines"\
 set ylabel "Temps de calcul (en secondes)"\

plot 'time_AQ.txt' using 1:2 with lines title 'AQ', \
      'time_HT.txt' using 1:2 with lines title 'HT', \
      'time_LC.txt' using 1:2 with lines title 'LC'

      */

    //pour l'arbre quaternaire et la table de hachage le temps il diminue de 0 à 1000 
    //pour la liste chainé ça grandit linéairement
    //à partir de 1000 le temps de calcul pour  l'arbre et la table augmentent 
    //plus la taille de la table est petite plus l'arbre quaternaire est mieux que le temps d'execution est très grand pour HT ( bcp de collisions)
    //plus la taille de la table  est grande moins on a de collisions et plus la table de hachage est performante comparé a l'arbre 
    
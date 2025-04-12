#include <stdio.h>
#include <stdlib.h>
#include "Chaine.h"
#include "Liste.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <fichier_entree> <fichier_sortie>\n", argv[0]);
        return 1;
    }

    // Ouvrir le fichier d'entrée en mode lecture
    FILE *f_entree = fopen(argv[1], "r");
    if (f_entree == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier de sortie.\n");
        return 1;
    }

    // Lire les données à partir du fichier d'entrée
    Chaines *C = lectureChaines(f_entree);


    //visualisation de la chaine 
    afficheChainesSVG(C , "chaineSVG");

    fclose(f_entree);

    // Ouvrir le fichier de sortie en mode écriture
    FILE *f_sortie = fopen(argv[2], "w");
    if (f_sortie == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier de sortie.\n");
        return 1;
    }

    // Écrire les données dans le fichier de sortie
    ecrireChaines(C, f_sortie);
    fclose(f_sortie);

    // Calcul et affichage de la longueur totale des chaînes
    double longueur_totale = longueurTotale(C);
    printf("\nLongueur totale des chaînes : %.2lf\n", longueur_totale);

    // Calcul et affichage du nombre total de points
    int nb_points_total = comptePointsTotal(C);
    printf("Nombre total de points : %d\n", nb_points_total);


    // Libérer la mémoire allouée pour la structure Chaines
    libererChaines(C);
    return 0;
}

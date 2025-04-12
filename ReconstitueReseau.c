#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Chaine.h"
#include "Liste.h" 
#include "Hachage.h"
#include "ArbreQuat.h"
// Fonction pour reconstituer le réseau en utilisant une méthode spécifiée

Reseau* reconstitueReseau(Chaines *C, int methode) {
    
    switch (methode) {
        case 0:
            return reconstitueReseauListe(C);
            break;
        case 1:
            return reconstitueReseauHachage(C,10);
            break;
        case 2:
            return reconstitueReseauArbre(C);
            break;
        default:
            fprintf(stderr, "Méthode non reconnue.\n");
            return NULL ;
    }
    return NULL ;
}

int main(int argc, char *argv[]) {
    // Vérifier le nombre d'arguments
    if (argc != 3) {
        printf("Usage: %s <fichier.cha> <methode>\n", argv[0]);
        printf("Méthodes disponibles:\n");
        printf("1 - Liste chaînée\n");
        printf("2 - Table de hachage\n");
        printf("3 - Arbre\n");
        return EXIT_FAILURE;
    }

    FILE* f1;
    f1 = fopen(argv[1],"r");  
    if (f1 == NULL) {
    fprintf(stderr, "Erreur lors de l'ouverture du fichier %s.\n", argv[1]);
    return 1;
    }

    Chaines* ch = lectureChaines(f1);

    int meth = atoi(argv[2]);

    Reseau *R = reconstitueReseau(ch,meth);

    fclose(f1);
    printf(" %p", R->commodites);

    printf("NbNoeuds: %d\nGamma: %d\n", R->nbNoeuds, R->gamma);

    CellNoeud *nodes = R->noeuds;
    while(nodes){
        printf("num du noeud: %d , x = %lf , y = %lf \n", nodes->nd->num, nodes->nd->x , nodes->nd->y);
        CellNoeud * voisin= nodes->nd->voisins;
        while(voisin){
            printf("num du noeud voisin: %d\n", voisin->nd->num);
            voisin= voisin->suiv;
        }
        nodes = nodes->suiv;
    }

    FILE* f=fopen("reseautest","w");
    ecrireReseau(R,f);

    CellCommodite *com = R->commodites;
    while(com){
        printf("num du noeud d'extrémité 1: %d\nnum du noeud d'extrémité 2: %d\n", com->extrA->num, com->extrB->num);
        com = com->suiv;
    }
    printf( "nombre_liasons =%d",nbLiaisons(R));

    afficheReseauSVG(R,"result");
    
    fclose(f);

    detruireReseau(R);
    libererChaines(ch);
    
    //la fonction clef est elle appropriée
    //for (double i =0;i<11;i++){
    //    for ( double j =0;j<11 ;j++){
    //        printf( " f(%f,%f) = %f\n", i,j,Clef(i,j));
     //   }
    //}

    //Question:  Est-ce que la fonction clef vous semble appropriée ?

    /*Les  clés générées pour les points (x, y) avec x
    entier allant de 1 à 10 et y entier allant de 1 à 10 semblent être dispersés sur une
    large gamme de valeurs, ce qui est souhaitable pour réduire les collisions et répartir les données de
    manière équilibrée dans la table de hachage.*/

    for (int x = 1; x <= 10; ++x) {
        for (int y = 1; y <= 10; ++y) {
            unsigned int resultat = Clef(x, y);
            printf("Point (%d, %d) -> Clef : %u\n", x, y, resultat);
        }
    }
    printf("fin de d'affichage du reseau et fin de test de la fonction clef \n");
    return 0;
}
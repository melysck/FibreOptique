#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Graphe.h"
#include "Reseau.h"
#include "Chaine.h"
#include "ArbreQuat.h"
#include "Hachage.h"

void test_reorganiseReseau();

int main() {
    test_reorganiseReseau();
    return 0;
}

void test_reorganiseReseau() {
        char *  fichier ="instance1.cha";
        FILE* f = fopen("instance1.cha", "r");
        if (f == NULL) {
            fprintf(stderr, "Erreur lors de l'ouverture du fichier %s.\n", fichier);
            return;
        }
        
        // Lecture des chaînes depuis le fichier
        Chaines* C = lectureChaines(f);
        fclose(f);

        // Reconstruire le réseau à partir des chaînes avec une liste chaînée
        Reseau* R_liste = reconstitueReseauArbre(C);

        printf("Petit test avant de commencer la réorganisation du réseau ");
        // Test de la fonction reorganiseReseau avec une liste chaînée
        Graphe * g = creerGraphe (R_liste);
        afficher_graphe (g);
        // printf("contenu :%p",  g->T_som[0]);
        printf(" plusPetitNombreAretes =%d : ", plusPetitNombreAretes(g, 4, 5));
        ListeChaine * chemin = cheminEntreDeuxSommets ( g , 4, 5);
        afficherListeChainee (chemin);

        int estReorganise_liste = reorganiseReseau(R_liste);

        printf(" %s (Liste chaînée) : ", fichier);

        if (estReorganise_liste) {
            printf("Réseau réorganisé\n");
        } else {
            printf("Réseau non réorganisé\n");
        }
        detruireReseau(R_liste);

        // Reconstruire le réseau à partir des chaînes avec une table de hachage

        Reseau* R_hachage = reconstitueReseauHachage(C, 10); 
        // Test de la fonction reorganiseReseau avec une table de hachage

        int estReorganise_hachage = reorganiseReseau(R_hachage);

        printf(" %s (Table de hachage) : ", fichier);
        if (estReorganise_hachage) {
            printf("Réseau réorganisé\n");
        } else {
            printf("Réseau non réorganisé\n");
        }
        detruireReseau(R_hachage);

        //Reconstruire le réseau à partir des chaînes avec un arbre quaternaire

        Reseau* R_arbre = reconstitueReseauArbre(C);

        //Test de la fonction reorganiseReseau avec un arbre quaternaire

        int estReorganise_arbre = reorganiseReseau(R_arbre);
        printf(" %s (Arbre quaternaire) : ", fichier);
        if (estReorganise_arbre) {
            printf("Réseau réorganisé\n");
        } else {
            printf("Réseau non réorganisé\n");
        }
        detruireReseau(R_arbre);

        //Libération de la mémoire
        libererChaines(C);
    
}
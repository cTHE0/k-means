/* Fichier    : k-means
 * Version    : 6
 * Date       : 04/12/24
 * Auteur     : Théo Couerbe
 * Plateforme : windows 11
 * Commande de compilation : gcc -Wall -Wextra -ansi -Wpedantic -o k-means k-means.c
 * Commande d'exécution : ./k-means
 */

#include <stdio.h>
#include <stdlib.h>
#include "100_points_de_dimension_2.txt"
#define K 4
struct point {
    double* coord;
    int cluster;
};

typedef struct point point;

double distance_carre(point p1, point p2, int dim) { /*Inutile d'effectuer une opération coûteuse avec la racine carré*/
    int d;
    double res;
    res = 0;
    for (d=0; d<dim; d++){
        res += (p1.coord[d]-p2.coord[d])*(p1.coord[d]-p2.coord[d]);
    }
    return res;
}

int ctr_le_plus_proche(int i, point *C, point *P, int dim, int k) {
    int j;
    int res = 0;
    double d = distance_carre(C[0], P[i], dim);

    for (j=1; j<k; j++) {
        double nv_d = distance_carre(C[j], P[i], dim);
        if ( nv_d < d) {
            res = j;
            d = nv_d;
        }
    }
    return res;
}

void ecrit_liste_points(point *L, int dim, int nb_pts, char *fichier){
    FILE *f;
    int i;
    int d;
    f = fopen(fichier, "w");
    if ( f == NULL ) {
        printf("Impossible d'écrire dans le fichier\n");
    }
    fputs("# 10_points.kmn \n", f);
    fprintf(f, "Nombre d'éléments: %d\n", nb_pts);
    fprintf(f, "Dimension: %d\n", dim);
    for (i=0; i<nb_pts; i++) {
        fprintf(f, "(%.2f", L[i].coord[0]);
        for (d=1; d<dim; d++) {
            fprintf(f, ", %.2f", L[i].coord[d]);
        }
        fprintf(f, ") [%d]\n", L[i].cluster);
    }
    fclose(f);
}


void exporte_resultats(point *C, point *P, int nb_pts, int k, int dim, int nb_op){
    ecrit_liste_points(P, dim, nb_pts, "10_points.kmn");
    ecrit_liste_points(C, dim, k, "10_points.ctr");
    printf("Résultat obtenu en %d opérations !", nb_op);
}

void modif_ctr(point *C, point *P, int dim, int k, int nb_pts){
    int i;
    int j;
    int d;
    double *sum = malloc(k*dim*sizeof(double));

    for (i=0; i<k; i++){
        for (d=0; d<dim; d++){
                *(sum + i*dim + d) = 0;
        }
    }

    for (i=0; i<k; i++){
        for (j=0; j<nb_pts; j++){
            if (P[j].cluster == i){
                for (d=0; d<dim; d++){
                    *(sum + i*dim + d) += P[j].coord[d];
                }
            }
        }
        for (d=0; d<dim; d++){
            if (C[i].cluster != 0){
                C[i].coord[d] = *(sum + i*dim + d) / C[i].cluster;
            }
        }
    }

    free(sum);
}

int calcule_centroides(point *C, point *P, int dim, int k, int nb_pts){
    int i = 0;
    int j;
    int change = 1;
    int c_j;
    while (change==1) {
        change = 0;
        for (j=0; j<nb_pts; j++) {
            c_j = ctr_le_plus_proche(j, C, P, dim, k);
            
            if (P[j].cluster != c_j) {
                C[P[j].cluster].cluster -= 1;
                C[c_j].cluster += 1;
                P[j].cluster = c_j;
                change = 1;
             }
        }
        modif_ctr(C, P, dim, k, nb_pts);
        i++;
    }
    return i;
}

point* initialise_liste_points(double pts[][DIM], int dim, int nb_pts) {
    int i;
    int d;
    point* Pts_res = malloc(nb_pts*sizeof(point));
    for (i=0; i<nb_pts; i++){
        Pts_res[i].cluster = 0;
        Pts_res[i].coord = (double*)malloc(dim*sizeof(double));
        for (d=0; d<dim; d++){
            Pts_res[i].coord[d] = pts[i][d];
        }
    }
    if (!Pts_res) {
        return NULL;
    }
    return Pts_res;
}

int main() {
    int i;
    int d;

    point* C;
    point* P;
    int nb_iterations;

    /* Initialisation de la liste des centroïdes */
    C = malloc(K*sizeof(point));
    for (i=0; i<K; i++){
        C[i].cluster = 0;
        C[i].coord = malloc(DIM*sizeof(double));
        for (d=0; d<DIM; d++){
            C[i].coord[d] = pts[i][d];
        }
    }
    C[0].cluster = NB_PTS;

    P = initialise_liste_points(pts, DIM, NB_PTS);

    nb_iterations = calcule_centroides(C, P, DIM, K, NB_PTS);

    exporte_resultats(C, P, NB_PTS, K, DIM, nb_iterations);

    for (i=0; i<K; i++){
        free(C[i].coord);
    }
    for (i=0; i<NB_PTS; i++){
        free(P[i].coord);
    }
    free(C);
    free(P);

    return 0;
}
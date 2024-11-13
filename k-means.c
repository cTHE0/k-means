/* Fichier    : k-means
 * Version    : 6
 * Date       : 13/11/24
 * Auteur     : Théo Couerbe
 * Plateforme : windows 11
 * Commande de compilation : gcc -Wall -Wextra -ansi -Wpedantic -o k-means_v6 k-means_v6.c
 * Commande d'exécution : ./k-means_v6
 */

#include <stdio.h>
#include <stdlib.h>
#define NB_PTS 10
#define K 3
#define DIM 2

struct point {
    double coord[DIM];
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

void initialise(point *C, point *P, int nb_pts, int k, int dim){
    int i;
    int d;
    point Pts[] = {
        {{-48.358319,  43.397062}, 0},
        {{-82.968798, 100.258353}, 0},
        {{102.162965, -77.655336}, 0},
        {{-29.703875, 121.367630}, 0},
        {{-1.497668,   0.269966}, 0},
        {{61.963342, -27.502030}, 0},
        {{38.378263, -12.302994}, 0},
        {{-3.953463,  -6.158928}, 0},
        {{-24.933400,  22.100650}, 0},
        {{16.373373, -53.232660}, 0}
    };
    point Ctr[] = {
    {{-48.358319,  43.397062}, 0},
    {{-82.968798, 100.258353}, 0},
    {{102.162965, -77.655336}, 0}
    };

    Ctr[0].cluster = nb_pts;

    for (i=0; i<nb_pts; i++){
        P[i].cluster = Pts[i].cluster;
        for (d=0; d<dim; d++){
            P[i].coord[d] = Pts[i].coord[d];
        }
    }
    for (i=0; i<k; i++){
        C[i].cluster = Ctr[i].cluster;
        for (d=0; d<dim; d++){
            C[i].coord[d] = Ctr[i].coord[d];
        }
    }
}

int main() {

    point C[K];
    point P[NB_PTS];
    int nb_iterations;

    initialise(C, P, NB_PTS, K, DIM);

    nb_iterations = calcule_centroides(C, P, DIM, K, NB_PTS);

    exporte_resultats(C, P, NB_PTS, K, DIM, nb_iterations);

    return 0;
}
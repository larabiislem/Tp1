#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "primes.h"

void tester(const char* fichier) {
    FILE* f = fopen(fichier, "r");
    if (!f) {
        printf("Impossible d'ouvrir : %s\n\n", fichier);
        return;
    }

    long long n;
    int total = 0;
    int nbPremiers = 0;
    int incoherences = 0;

    while (fscanf(f, "%lld", &n) == 1) {
        int a1 = estPremier_A1(n);
        int a2 = estPremier_A2(n);
        int a3 = estPremier_A3(n);
        int a4 = estPremier_A4(n);

        total++;

        if (a1 == 1 && a2 == 1 && a3 == 1 && a4 == 1) nbPremiers++;

        if (!(a1 == a2 && a2 == a3 && a3 == a4)) incoherences++;
    }

    printf("============================================================\n");
    printf("Fichier : %s\n", fichier);
    printf("------------------------------------------------------------\n");
    printf("Total testés              : %d\n", total);
    printf("Nombres premiers          : %d\n", nbPremiers);
    if (total > 0)
        printf("Pourcentage de premiers   : %.2f %%\n", (nbPremiers * 100.0) / total);
    printf("Incohérences algorithmes  : %d\n", incoherences);
    printf("============================================================\n\n");

    fclose(f);
}

int main() {
    tester("dataset/Random100.txt");
    tester("dataset/Random1000.txt");
    tester("dataset/Test-1.txt");
    tester("dataset/Test-2.txt");
    tester("dataset/Test-3.txt");
    return 0;
}

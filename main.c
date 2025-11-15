#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "primes.h"

void tester(const char* fichier) {
    FILE* f = fopen(fichier, "r");
    if (!f) return;

    long long n;
    printf("Fichier : %s\n", fichier);

    while (fscanf(f, "%lld", &n) == 1) {
        int a1 = estPremier_A1(n);
        int a2 = estPremier_A2(n);
        int a3 = estPremier_A3(n);
        int a4 = estPremier_A4(n);

        printf("%lld | A1=%d | A2=%d | A3=%d | A4=%d\n", n, a1, a2, a3, a4);
    }

    printf("\n");
    fclose(f);
}

int main() {
    tester("./dataset/Random100.txt");
    tester("./dataset/Random1000.txt");
    tester("./dataset/Test-1.txt");
    tester("./dataset/Test-2.txt");
    tester("./dataset/Test-3.txt");
    return 0;
}

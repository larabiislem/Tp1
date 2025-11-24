#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long long generer_nombre(int digits) {
    long long min = 1;
    for (int i = 1; i < digits; i++) min *= 10;
    long long max = min * 10 - 1;
    return min + rand() % (max - min + 1);
}

int main() {
    FILE *f = fopen("./dataset/Test-3.txt", "w");
    if (!f) return 1;

    srand(time(NULL));

    int tailles[] = {3, 6, 9, 12};
    int total = 300;
    int par_taille = total / 4;

    for (int i = 0; i < 4; i++) {
        int digits = tailles[i];
        for (int k = 0; k < par_taille; k++) {
            long long x = generer_nombre(digits);
            fprintf(f, "%lld\n", x);
        }
    }

    fclose(f);
    return 0;
}

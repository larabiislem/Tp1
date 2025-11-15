#include "primes.h"
#include <math.h>

int estPremier_A1(long long N) {
    if (N < 2) return 0;
    for (long long i = 2; i < N; i++) {
        if (N % i == 0) return 0;
    }
    return 1;
}

int estPremier_A2(long long N) {
    if (N < 2) return 0;
    for (long long i = 2; i <= N / 2; i++) {
        if (N % i == 0) return 0;
    }
    return 1;
}

int estPremier_A3(long long N) {
    if (N < 2) return 0;
    long long limite = sqrt(N);
    for (long long i = 2; i <= limite; i++) {
        if (N % i == 0) return 0;
    }
    return 1;
}

int estPremier_A4(long long N) {
    if (N < 2) return 0;
    if (N == 2) return 1;
    if (N % 2 == 0) return 0;
    long long limite = sqrt(N);
    for (long long i = 3; i <= limite; i += 2) {
        if (N % i == 0) return 0;
    }
    return 1;
}

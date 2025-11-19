#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "primes.h"

#ifdef _WIN32
#include <windows.h>
static double now_sec(void)
{
    static LARGE_INTEGER freq;
    static int init = 0;
    LARGE_INTEGER t;
    if (!init)
    {
        QueryPerformanceFrequency(&freq);
        init = 1;
    }
    QueryPerformanceCounter(&t);
    return (double)t.QuadPart / (double)freq.QuadPart;
}
#else
static double now_sec(void)
{
    return (double)clock() / CLOCKS_PER_SEC;
}
#endif

static int run_and_time(int (*isprime)(long long), long long n, double *elapsed)
{
    double t0 = now_sec();
    int r = isprime(n);
    double t1 = now_sec();
    if (elapsed)
        *elapsed = t1 - t0;
    return r;
}

static void process_file(const char *path)
{
    FILE *fp = fopen(path, "r");
    if (!fp)
    {
        fprintf(stderr, "Cannot open %s\n", path);
        return;
    }

    int (*algos[4])(long long) = {estPremier_A1, estPremier_A2, estPremier_A3, estPremier_A4};
    char buf[256];
    while (fgets(buf, sizeof(buf), fp))
    {
        char *p = buf;
        while (*p == ' ' || *p == '\t')
            ++p;
        if (*p == '\0' || *p == '\n' || *p == '#')
            continue;
        char *endptr = NULL;
        long long N = strtoll(p, &endptr, 10);
        if (endptr == p)
            continue;
        for (int a = 0; a < 4; ++a)
        {
            double elapsed;
            int res = run_and_time(algos[a], N, &elapsed);
            printf("%s,%lld,A%d,%d,%.9f\n", path, N, a + 1, res, elapsed);
        }
    }

    fclose(fp);
}

int main(void)
{
    const char *files[] = {
        "dataset/Random100.txt",
        "dataset/Random1000.txt"};

    printf("File,N,Algorithm,Result,Time(s)\n");

    for (size_t i = 0; i < sizeof(files) / sizeof(files[0]); ++i)
        process_file(files[i]);

    return 0;
}

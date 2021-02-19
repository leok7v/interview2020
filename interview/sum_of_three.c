#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Problem statement:
// given array of positive distinct integers (in random order)
// count number of tripples (not necesseraly sequential) that adds
// to specified sum:
//    int sum3_count_O3(int a[], int n)
// array can be rewritten in place during counting

#undef VERBOSE // define for debuggins, too much output and too slow for test run

static bool verbose; // initialized as false by linker, loader

#define countof(a) (sizeof(a) / sizeof(a[0]))

static void test(void);

static int compare(const void* a, const void* b) { return *(int*)a - *(int*)b; }

#ifdef VERBOSE
#    define trace(format, ...) printf(format, ## __VA_ARGS__)
#    define traceln(format, ...) printf(format "\n", ## __VA_ARGS__)
#else
#    define trace(format, ...) do { if (verbose) { printf(format "\n", ## __VA_ARGS__); } } while (false)
#    define traceln(format, ...) do { if (verbose) { printf(format "\n", ## __VA_ARGS__); } } while (false)
#endif

static void printa(int* a, int n, int s) {
    trace("s=%d a[%d]={", s, n);
    for (int i = 0; i < n; i++) { trace("%d%c", a[i], i < n - 1 ? ',' : '}'); }
    trace("\n");
}

static int sum3_count_O3(int* a, int n, int s) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            for (int k = j + 1; k < n; k++) {
                if (a[i] + a[j] + a[k] == s) {
                    traceln("a[%d]=%d + a[%d]=%d + a[%d]=%d == %d", i, a[i], j, a[j], k, a[k], s);
                }
                count += a[i] + a[j] + a[k] == s;
            }
        }
    }
    traceln("sum count=%d O(n^3)", count);
    return count;
}

static int sum3_count_ONlnN(int* a, int n, int s) {
    qsort(a, n, sizeof(a[0]), compare);
    int count = 0;
    int i = 0;
    int j = i + 1;
    int k = n - 1;
    while (i < j && j < k) {
        while (i < k - 1 && a[i] + a[i + 1] + a[k] > s) { k--; }
        while (j < k && a[i] + a[j] + a[k] < s) { j++; }
        while (j < k && a[i] + a[j] + a[k] > s) { k--; }
        if (j < k && a[i] + a[j] + a[k] == s) {
            count++;
            traceln("a[%d]=%d + a[%d]=%d + a[%d]=%d == %d", i, a[i], j, a[j], k, a[k], s);
            // there is no more tripples inside [i..k] that
            // can sum to "s" because all numbers are distinct
            if (j < k - 1) {
                k--;
            } else {
                i++;
                j = i + 1;
                while (k < n - 1 && a[i] + a[j] + a[k + 1] <= s) { k++; }
            }
        } else if (j == k) {
            if (k < i + 2) {
               k--;
               if (j > k - 1) { j = k - 1; }
            } else if (i < n - 2){
               i++;
               j = i + 1;
               while (k < n - 1 && a[i] + a[j] + a[k + 1] <= s) { k++; }
            } else {
               break;
            }
        }
    }
    traceln("sum count=%d O(n * ln(n))", count);
    return count;
}

static int run(int* a, int n, int sum) {
    int c3 = sum3_count_O3(a, n, sum);
    int cn = sum3_count_ONlnN(a, n, sum);
    if (c3 != cn) { // useful for debugging:
        verbose = true;
        printa(a, n, sum);
        c3 = sum3_count_O3(a, n, sum);
        cn = sum3_count_ONlnN(a, n, sum);
    }
    assert(c3 == cn);
    return c3 == cn ? 0 : EFAULT;
}

int main(int argc, const char * argv[]) {
    int a[] = {13, 2, 3, 6, 4, 7, 9, 10, 1};
    //  a[] = {1, 2, 3, 4, 6, 7, 9, 10, 13} sorted
    int b[] = {6,16,18,25,30,38,58,60,73,89};
    int r = run(a, countof(a), 13); // 13 == 1 + 2 + 10, 2 + 4 + 9, 0 + 3 + 10, ...
    r = r == 0 ? run(b, countof(a), 58) : r;
    r = r == 0 ? run(b, countof(a), 59) : r;
    r = r == 0 ? run(b, countof(a), 61) : r;
    r = r == 0 ? run(b, countof(a), 82) : r;
    test();
    return 0;
}

static void fill_array(int* a, int n, int range) {
    for (int i = 0; i < n; i++) {
        bool distinct;
        int r;
        do {
            distinct = true;
            r = random() % range + 1;
            for (int j = 0; j < i && distinct; j++) { distinct = a[j] != r; }
        } while (!distinct);
        a[i] = r;
    }
}

static double seconds() {
    struct timespec ts = {};
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-09;
}

static void performance() {
    enum { N = 10 };
    int a[1000];
    const int n = countof(a);
    const int sum = n / 2;
    fill_array(a, n, n * 3 / 2);
    double time = seconds();
    for (int i = 0; i < N; i++) { sum3_count_O3(a, n, sum); }
    double elapsed_o3 = (seconds() - time) * 1000;
    enum { M = 1000 };
    time = seconds();
    for (int i = 0; i < N; i++) { sum3_count_ONlnN(a, n, sum); }
    double elapsed_ln = (seconds() - time) * 1000;
    fprintf(stderr, "performance n=%d O(n^3) %.6f milliseconds O(n * ln(n)) %.6f milliseconds\n",
            n, elapsed_o3 / N, elapsed_ln / M);
}

static void test_n(const int n, const int iterations) {
    int* a = (int*)alloca(n * sizeof(int));
    for (int pass = 0; pass < iterations; pass++) {
        fill_array(a, n, 98); // [1..99] easy on the eyes to debug
        for (int s = 3; s < 99 * 3; s++) {
            if (sum3_count_O3(a, n, s) != sum3_count_ONlnN(a, n, s)) {
                verbose = true;
                printa(a, n, s);
                (void)(sum3_count_O3(a, n, s) != sum3_count_ONlnN(a, n, s)); // in verbose mode
                fprintf(stderr, "TEST FAILED\n");
                exit(1);
            }
        }
    }
}

static void test(void) {
    for (int n = 3; n <= 32; n++) { test_n(n, 1000); }
    fprintf(stderr, "TEST PASSED\n");
    performance();
}

// performance n=1000 O(n^3) 242.420400 milliseconds O(n * ln(n)) 0.001085 milliseconds
// Model Name:        MacBook Pro
// Model Identifier:  MacBookPro8,3
// Processor Name:    Intel Core i7
// Processor Speed:   2.5 GHz
// Number of Processors :    1
// Total Number of Cores:    4
// L2 Cache (per Core):    256 KB
// L3 Cache:  8 MB
// Memory:    8 GB
// leo.kuznetsov@gmail.com
// Nov 19, 2020

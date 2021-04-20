#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define GLOBALS 1
#include "real.h"
// #include
// extern int dig_in;
// dig_ing = 1;

extern int dig_in;

static long get_nanos(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (long)ts.tv_sec * 1000000ull + ts.tv_nsec;
}

int main(void) {
    long nanos;
    long last_nanos;
    long start;
    nanos = get_nanos();
    last_nanos = nanos;
    start = nanos;
    // int dig_in;
    // char *c = "";
    long time_dif;
    float dig1;
    char str[3];

    // while (1) {
    //     nanos = get_nanos();
    //     if (nanos - last_nanos > 1000000ull) {
    //         printf("current nanos: %ld\n", nanos - start);
    //         last_nanos = nanos;
    //     }
    // }
    while (1) {
        
        if ((dig_in != 0)){
            nanos = get_nanos();
            time_dif = nanos - last_nanos;
            printf("nanos: %ld & last_nanos: %ld\n", nanos, last_nanos);
            printf("Nanos from last peak: %ld\n", time_dif);
            last_nanos = nanos;

            return EXIT_SUCCESS;
        } else {
            // printf("skipping");
           
            printf("%d", dig_in);
        }
    }
    return EXIT_SUCCESS;
}
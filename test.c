#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <sys/mman.h>

#define GLOBALS 1
#include "real.h"
// #include
// extern int dig_in;
// dig_ing = 1;

// extern int dig_in;
int val;
// int *a = (int*)0x22cd28;

// extern unsigned int addr;

static long get_nanos(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (long)ts.tv_sec * 1000000ull + ts.tv_nsec;
}

int main(void) {
    // int * const mem1 = (int *)0x9000;
    
    // printf("%p\n", &a);
    // val = (int)*a;
    // printf("%d", val);
    // printf("%x", &a);

    printf("value of num = %d\n", *a);

    long nanos;
    long last_nanos;
    long start;
    nanos = get_nanos();
    last_nanos = nanos;
    start = nanos;
    // int dig_in;
    // char *c = "";
    long time_dif;
    // float dig1;
    char str[3];

    // while (1) {
    //     nanos = get_nanos();
    //     if (nanos - last_nanos > 1000000ull) {
    //         printf("current nanos: %ld\n", nanos - start);
    //         last_nanos = nanos;
    //     }
    // }
    while (1) {
        // val = (int)*a;
        val == 1;
        if ((val == 88)){
            printf("1");
            nanos = get_nanos();
            time_dif = nanos - last_nanos;
            printf("nanos: %ld & last_nanos: %ld\n", nanos, last_nanos);
            printf("Nanos from last peak: %ld\n", time_dif);
            last_nanos = nanos;

            return EXIT_SUCCESS;
        }
        // } else {
        //     // printf("skipping");
           
        //     printf("%d", dig_in);
        // }
        if ((val == 16)){
            printf("0");
        }
    }
    return EXIT_SUCCESS;
}
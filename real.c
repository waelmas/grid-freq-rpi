#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "real.h"
// #include
// #include <omp.h>

// static long get_nanos(void) {
//     struct timespec ts;
//     timespec_get(&ts, TIME_UTC);
//     return (long)ts.tv_sec * 1000000ull + ts.tv_nsec;
// }
// int dig_in = 0;
// extern dig_in;
// extern int dig_in;
int main(void) {
    // long nanos;
    // long last_nanos;
    // long start;
    // nanos = get_nanos();
    // last_nanos = nanos;
    // start = nanos;
    // int dig_in;
    // char *c = "";
    // long time_dif;
    // int dig_in;

    // while (1) {
    //     nanos = get_nanos();
    //     if (nanos - last_nanos > 1000000ull) {
    //         printf("current nanos: %ld\n", nanos - start);
    //         last_nanos = nanos;
    //     }
    // }
    // #pragma omp parallel for
    while (1) {
        // nanos = get_nanos();
        char c = getchar();
        if ((c != "")){
            dig_in = 1;
            printf("1");
        } else {
            dig_in = 0;

        }
        // if ((dig_in != 0)){
        //     nanos = get_nanos();
        //     time_dif = nanos - last_nanos;
        //     printf("nanos: %ld & last_nanos: %ld\n", nanos, last_nanos);
        //     printf("Nanos from last peak: %ld\n", time_dif);
        //     last_nanos = nanos;
        // } else {
        //     printf('skipping');
        // }
    }
    
    // #pragma omp parallel
    // while (1) {
    //     if ((dig_in != 0)){
    //         nanos = get_nanos();
    //         time_dif = nanos - last_nanos;
    //         printf("nanos: %ld & last_nanos: %ld\n", nanos, last_nanos);
    //         printf("Nanos from last peak: %ld\n", time_dif);
    //         last_nanos = nanos;
    //     } else {
    //         printf('skipping');
    //     }
    // }


    return EXIT_SUCCESS;
}
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
int dig_in;

void change(); 
void change0(); 

int main(int argc, char **argv) {
    char c;

    

    while (1) {
        // nanos = get_nanos();
        char c = getchar();
        // printf("%c\n", c);
        if ((c == 'a')){
            // dig_in = 1;
            // printf()
            change();
        }
        // } else {
        //     // dig_in = 0;
        //     change0();
        if ((c == 's')){
            change0();
        }
            
  

        // }
    }

    return EXIT_SUCCESS;
}

// int main (int argc, char **argv){





//   char c = getchar();
//   printf("Char: %c", c);
//   return 0;
// }

void change(){ 
	//just assigning 1
    printf("changed\n");
	dig_in = 1;				 
} 
void change0(){ 
	//just assigning 1
    printf("back to zero\n");
	dig_in = 0;				 
} 
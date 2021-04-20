#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>

#include <sys/mman.h>

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
int val;
// char mem1;
unsigned int addr;

int * const mem1 = (int *)0xe193c998;
char *a;

void change(); 
void change0(); 



int main(int argc, char **argv) {
    char c;
    
    
    a = (char*)malloc(sizeof(char) * 1);
    
    // mem1 = &a;
    // int * const mem1 = dig_in;

    
    // int *a = (int*)0x22cd28;
    // *a = 1;
    val = (int)*a;
    
    // printf("%s", mem1);
    addr = &mem1;
    // printf("%p\n", &a);
    printf("value of num = %d\n", *a);

    // printf("%x", addr);
    // char buf[100];
    // sprintf(buf, "%" PRIuPTR, (uintptr_t) &a);
    // printf("The address of x is %s.\n", buf);
    
    // //  Read the address.
    // uintptr_t u;
    // sscanf(buf, "%" SCNuPTR, &u);

    // //  Convert the integer value to an address.
    // int *p = (int *) u;

    // //  Modify the int through the new pointer.
    // *p = 123;

    // //  Display the int.
    // printf("x = %d\n", a);
    

    while (1) {
        // nanos = get_nanos();
        char c = getchar();
        // printf("%c\n", c);
        if ((c == 'a')){
            // dig_in = 1;
            // printf()
            change(&a);
            val = (int)*a;
            printf("From Mem: %d\n", val);

        }
        // } else {
        //     // dig_in = 0;
        //     change0();
        if ((c == 's')){
            change0(&a);
            val = (int)*a;
            printf("From Mem: %d\n", val);
        }
            
  

        // }
    }

    free(a);

    return EXIT_SUCCESS;
}

// int main (int argc, char **argv){





//   char c = getchar();
//   printf("Char: %c", c);
//   return 0;
// }

void change(char *a){ 
	//just assigning 1
    // char* a = &a;
    // a = (char*)malloc(sizeof(char) * 1);
    printf("changed\n");
	dig_in = 1;
    *a = 1;		 
} 
void change0(char *a){ 
	//just assigning 1
    // char* a = &a;
    // a = (char*)malloc(sizeof(char) * 1);
    printf("back to zero\n");
	dig_in = 0;	
    *a = 0;			 
} 
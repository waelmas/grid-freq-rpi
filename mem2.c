#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

char dig_in;

void change(); 
void change0(); 

void* create_shared_memory(size_t size) {
  // Our memory buffer will be readable and writable:
  int protection = PROT_READ | PROT_WRITE;

  // The buffer will be shared (meaning other processes can access it), but
  // anonymous (meaning third-party processes cannot obtain an address for it),
  // so only this process and its children will be able to use it:
  int visibility = MAP_SHARED | MAP_ANONYMOUS;

  // The remaining parameters to `mmap()` are not important for this use case,
  // but the manpage for `mmap` explains their purpose.
  return mmap(NULL, size, protection, visibility, -1, 0);
}

void change(){ 
	//just assigning 1
    // char* a = &a;
    // a = (char*)malloc(sizeof(char) * 1);
    printf("changed\n");
	dig_in = '1';
    	 
} 
void change0(){ 
	//just assigning 1
    // char* a = &a;
    // a = (char*)malloc(sizeof(char) * 1);
    printf("back to zero\n");
	dig_in = '0';	
    		 
}

static long get_nanos(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (long)ts.tv_sec * 1000000ull + ts.tv_nsec;
}





int main() {
  char parent_message[] = "000000";  // parent process will write this message
  char child_message1[] = "111111"; // child process will then write this one
  char child_message0[] = "000000"; // child process will then write this one
  char str1[15];

  int comp1;

  void* shmem = create_shared_memory(48);

  memcpy(shmem, parent_message, sizeof(parent_message));

//   memcpy(shmem, child_message0, sizeof(child_message0));

  int tt;

      long nanos;
    long last_nanos;
    long start;
    nanos = get_nanos();
    last_nanos = nanos;
    start = nanos;
    // int dig_in;
    // char *c = "";
    long time_dif;
    int last_val = 0;

  int pid = fork();


  
    // tt = sizeof(shmem);
    // printf("shmem: %d\n", tt);
    //  int   tt1 = sizeof(child_message1);
    // printf("ON: %d\n", tt1);
    //   int  tt0 = sizeof(child_message0);
    // printf("OFF: %d\n", tt0);

    // strcpy(str1, shmem);
    // comp1 = strncmp(str1, child_message1, 4);
    // printf("COMP to ON: %d\n string: %s\n", comp1, str1);

while (1) {

    if (pid == 0) {
        // printf("Sub 1 : %s\n", shmem);
        memcpy(shmem, child_message0, sizeof(child_message0));
        // printf("Sub 1: %s\n", shmem);
            tt = sizeof(shmem);
    // printf("shmem 22222: %d\n", tt);
        sleep(1);
        memcpy(shmem, child_message1, sizeof(child_message1));
        // printf("Sub 1: %s\n", shmem);
        sleep(1);
        memcpy(shmem, child_message0, sizeof(child_message0));
        // printf("Sub 1: %s\n", shmem);
        sleep(1);
        memcpy(shmem, child_message1, sizeof(child_message1));
        // printf("Sub 1: %s\n", shmem);
        sleep(1);
        memcpy(shmem, child_message0, sizeof(child_message0));
        // printf("Sub 1: %s\n", shmem);
        sleep(1);
        memcpy(shmem, child_message1, sizeof(child_message1));
        // printf("Sub 1: %s\n", shmem);
        sleep(1);
        memcpy(shmem, child_message0, sizeof(child_message0));
        // printf("Sub 1: %s\n", shmem);
        sleep(1);
        memcpy(shmem, child_message1, sizeof(child_message1));
        sleep(1);
        // printf("Sub 1: %s\n", shmem);

    } else {
        // printf("Parent read: %s\n", shmem);
        comp1 = strncmp(child_message1, shmem, 4);

        if (comp1 == 0){
            // printf("ON\n %s \n", shmem);
            

            if (last_val == 0){
                nanos = get_nanos();
                time_dif = nanos - last_nanos;
                printf("nanos: %ld & last_nanos: %ld\n", nanos, last_nanos);
                printf("Nanos from last peak (HALF PERIOD): %ld\n", time_dif/2);
                last_nanos = nanos;
                }
            last_val = 1;
        } else {
            //  printf("OFF\n %s \n", shmem);
             last_val = 0;
        }

    }
    }
}
#include <linux/gpio.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

char dig_in;

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


static unsigned long long get_nanos(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (unsigned long long)ts.tv_sec * 1000000000ull + ts.tv_nsec;
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

    unsigned long long nanos;
    unsigned long long last_nanos;
    unsigned long long start;
    nanos = get_nanos();
    last_nanos = nanos;
    start = nanos;
    // int dig_in;
    // char *c = "";
    unsigned long long time_dif;
    int last_val = 0;

  int pid = fork();


while (1) {

    if (pid == 0) {

       //Opening device and getting file descriptor.
        int fd = open("/dev/gpiochip0",O_RDONLY);

        //structure for holding chip information
        //This structure is defined in /usr/include/linux/gpio.h
        struct gpiochip_info cinfo;

        //Getting the chip information via the ioctl system call
        //GPIO_GET_CHIPINFO_IOCTL defined also in /usr/include/linux/gpio.h
        int ret = ioctl(fd,GPIO_GET_CHIPINFO_IOCTL,&cinfo);

        //print out the chip information
        fprintf(stdout, "GPIO chip: %s, \"%s\", %u GPIO lines\n",
            cinfo.name, cinfo.label, cinfo.lines);

        //structure for holding line information.
        //structure defined in /usr/include/linux/gpio.h
        struct gpioline_info linfo;

        //get generic line information from system call
        ret = ioctl(fd,GPIO_GET_LINEINFO_IOCTL, &linfo);

        //Not sure what this line_offset is, but we specify the gpio number later.
        fprintf(stdout,"line %2d: %s\n",linfo.line_offset,linfo.name);

        //Reading lines
        //Set up some handles for requests and data
        struct gpiohandle_request req;
        struct gpiohandle_data data;

        //Although req and data can read multiple gpios at a time, we'll use just one
        //This reads line offset 22, which corresponds to the BCM value in "gpio readall"
        //This corresponds to Physical Pin 15
        req.lineoffsets[0] = 22;
        //have to indicate how many lines we are reading.
        req.lines = 1;
        //Make this an input request
        req.flags = GPIOHANDLE_REQUEST_INPUT;

        //Optionally give the line a name
        strcpy(req.consumer_label, "First Switch");

        //Get a line handle. Note that req.fd is provided a handle for the next ioctl. 
        int lhfd = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &req);




while(1){

            ret = ioctl(req.fd,  GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data);

          
            if(data.values[0]){
                // printf(child_message1);
                memcpy(shmem, child_message1, sizeof(child_message1));
            } else{
                // printf(child_message0);
                memcpy(shmem, child_message0, sizeof(child_message0));
            }
         
}



    } else {
        // printf("Parent read: %s\n", shmem);
        comp1 = strncmp(child_message1, shmem, 4);

        if (comp1 == 0){
            // printf("RAM: %s \n", shmem);
            

            if (last_val == 0){
                nanos = get_nanos();
                time_dif = nanos - last_nanos;
                // printf("nanos: %llu & last_nanos: %llu\n", nanos, last_nanos);
                printf("Nanos from start of previous peak: %09llu\n", time_dif);
                last_nanos = nanos;
                }
            last_val = 1;
        } else if (last_val == 1){
            //  printf("OFF\n %s \n", shmem);
             last_val = 0;
        }
    // usleep(250000);
    }
    }
}
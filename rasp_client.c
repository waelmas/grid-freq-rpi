/* client.c */
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



#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <time.h>




#pragma pack(1)

typedef struct payload_t {
    uint32_t id;
    uint32_t counter;
    // uint32_t period;
    unsigned long long int period;
} payload;

#pragma pack()


void sendMsg(int sock, void* msg, uint32_t msgsize)
{
    if (write(sock, msg, msgsize) < 0)
    {
        printf("Can't send message.\n");
        close(sock);
        exit(1);
    }
    printf("Sent (%d bytes) bytes.\n", msgsize);
    return;
}


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


void sleep_before_next1(void){
    struct timespec ts;
    int res;
    ts.tv_sec = 0; // 0 seconds
    ts.tv_nsec =  200000; // nanoseconds
    res = nanosleep(&ts, &ts);
}

void sleep_before_next2(void){
    struct timespec ts;
    int res;
    ts.tv_sec = 0; // 0 seconds
    ts.tv_nsec =  8803921; // nanoseconds
    res = nanosleep(&ts, &ts);
}


int main()
{

    char parent_message[] = "000000";  // parent process will write this message
    char child_message1[] = "111111"; // child process will then write this one
    char child_message0[] = "000000"; // child process will then write this one
    char str1[15];

    int comp1;

    void* shmem = create_shared_memory(48);

    memcpy(shmem, parent_message, sizeof(parent_message));


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
    unsigned long long nanos_now;
    int last_val = 0;
    int i = 0;



    const int PORT = 2300;
    const char* SERVERNAME = "localhost";
    int BUFFSIZE = sizeof(payload);
    char buff[BUFFSIZE];
    int sock;
    int nread;
    // float mintemp = -10.0;
    // float maxtemp = 30.0;
    time_t t;




    srand((unsigned) time(&t));
    
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, SERVERNAME, &server_address.sin_addr);
    server_address.sin_port = htons(PORT);

    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        printf("ERROR: Socket creation failed\n");
        return 1;
    }

    if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        printf("ERROR: Unable to connect to server\n");
        return 1;
    }

    printf("Connected to %s\n", SERVERNAME);

    payload comdata;



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
                // after we got a peak, we wait before reading again to save CPU
                sleep_before_next1();
                
            } else{
                // printf(child_message0);
                memcpy(shmem, child_message0, sizeof(child_message0));
                sleep_before_next2();
            }

            
         
}



    } else {
        // printf("Parent read: %s\n", shmem);
        comp1 = strncmp(child_message1, shmem, 4);

        if (comp1 == 0){
            // printf("RAM: %s \n", shmem);
            
            nanos_now = get_nanos();
            if (nanos_now - last_nanos > 8000000){
                nanos = get_nanos();
                time_dif = nanos - last_nanos;
                // printf("nanos: %llu & last_nanos: %llu\n", nanos, last_nanos);
                printf("Nanos from start of previous peak: %09llu\n", time_dif);
                last_nanos = nanos;

                comdata.id = i;
                comdata.counter = i;
                comdata.period = time_dif;

                sendMsg(sock, &comdata, sizeof(payload));
                bzero(buff, BUFFSIZE);
                // nread = read(sock, buff, BUFFSIZE);
                // payload *p = (payload*) buff;
                // printf("Received id=%d, counter=%d, period=%09llu\n",
                // p->id, p->counter, p->period);




                }
            last_val = 1;
        } else if (nanos_now - last_nanos <= 8000000){
            //  printf("OFF\n %s \n", shmem);
             last_val = 0;
             // after we got a peak, we wait before reading again to save CPU
             sleep_before_next2();
        }
    // usleep(250000);
    }
    i = i+1;
    }
    
    // close the socket
    close(sock);
    return 0;

}
/****
* gpio-read.c -- example program for reading gpio values using the <linux/gpio.h> C API
* The API consists primarily of data structures used as parameters to system calls.
* Adapted from from a youtube video of Linus Walleij at the Embedded Linux Conference Europe
* GPIO for Engineers and Makers: starting at timestamp 44:11
* For simplicity, there is no error checking.
****/

//Need the gpio API and support for the system calls.
#include <linux/gpio.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>


int main(){


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
//This reads line offset 4, which corresponds to the BCM value in "gpio readall"
req.lineoffsets[0] = 22;
//have to indicate how many lines we are reading.
req.lines = 1;
//Make this an input request
req.flags = GPIOHANDLE_REQUEST_INPUT;

//Optionally give the line a name
strcpy(req.consumer_label, "First Switch");

//Get a line handle. Note that req.fd is provided a handle for the next ioctl. 
int lhfd = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &req);

//Loop with some delay that keeps checking the input and displaying the value
for(int ii = 0; ii < 1000; ++ii){

    ret = ioctl(req.fd,  GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data);

    printf("line %d is %s\n",req.lineoffsets[0], data.values[0] ? "high" : "low");
    //wait 0.25 seconds
    usleep(250000);
}

return 0;
}
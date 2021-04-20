#include <sys/time.h>
int main(int argc, char **argv)
{
int distance;
struct timeval start,end;;
double elapsed ;
// Set up gpi pointer for direct register access
setup_io();
INP_GPIO(23); // must use INP_GPIO before we can use OUT_GPIO
OUT_GPIO(23);
INP_GPIO(7); //input for echo pin

GPIO_CLR = (1 << 23);
usleep(30000) // allow module to settle

printf("starting measurement")
GPIO_SET = (1<<23);
usleep(10); // send pulse 10us 
GPIO_CLR = (1<<23);


while ((GPIO_CLR == ( 0<<7)))
{} 
gettimeofday(&start,NULL);
while ((GPIO_SET == ( 1<<7))) 
{} // input HIGH
gettimeofday(&end,NULL);
elapsed = (end.tv_sec - start.tv_sec);
distance = (elapsed)/2 ; //in cm
printf("distance: %d cm¥n", distance);


return 0;

}
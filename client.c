/* client.c */

#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(1)

typedef struct payload_t {
    uint32_t id;
    uint32_t counter;
    uint32_t period;
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
    printf("Message sent (%d bytes).\n", msgsize);
    return;
}

int main()
{
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

    payload data;
    for(int i = 0; i < 5; i++) {
        data.id = 1;
        data.counter = i;
        data.period = 2938345;

        printf("\nSending id=%d, counter=%d, period=%d\n", data.id, data.counter, data.period);
        sendMsg(sock, &data, sizeof(payload));

        bzero(buff, BUFFSIZE);
        nread = read(sock, buff, BUFFSIZE);
        printf("Received %d bytes\n", nread);
        payload *p = (payload*) buff;
        printf("Received id=%d, counter=%d, period=%d\n",
                p->id, p->counter, p->period);
    }
    
    // close the socket
    close(sock);
    return 0;

}
/*
Simple udp client with stop and wait functionality
*/
#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<signal.h>

#define BUFLEN 512 // Max length of buffer
#define PACKETLEN 32
#define PORT 8882 // The port on which to send data
#define TIMEOUT 1 // 1 second

typedef struct packet1 {
  int sequence;
  int offset;
} ACK_PKT;

typedef struct packet2 {
  int sequence;
  int client;
  int offset;
  char data[BUFLEN];
} DATA_PKT;

void die(char * s) {
  perror(s);
  exit(1);
}

int populateBuffer(FILE* fp, DATA_PKT* pkt){
    int length = fread(pkt->data, sizeof(char), PACKETLEN, fp);
    pkt->sequence++;
    pkt->offset += PACKETLEN;
    if(length != PACKETLEN) {
        pkt->data[length] = 0;
        return 1;   // indicates that the file ended
    }
    return 0;
}

void transmit(int socketfd, DATA_PKT* pkt, struct sockaddr_in *server_socket){
    ACK_PKT ack;

    struct timeval timeout;
    timeout.tv_sec = TIMEOUT;
    timeout.tv_usec = 0;
    int slen = sizeof(struct sockaddr_in);

    /* 
        This is an infinite loop and will keep sending
        the data packet until an ack is received
     */
    while(1){
        printf("trying again \n");
        fflush(stdout);
        if (sendto(socketfd, pkt, sizeof(DATA_PKT), 0, (struct sockaddr * ) server_socket, slen) == -1)
            die("sendto()");

        // put timeout
        if (setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) 
            die("setsockopt()");

        if (recvfrom(socketfd, &ack, sizeof(ACK_PKT), 0, (struct sockaddr * ) server_socket, &slen) == -1) 
            continue;
    } 
}

int main(void) {
    struct sockaddr_in server_socket;
    int clientfd, i, slen = sizeof(server_socket);
    
    DATA_PKT pkt, rcv_ack;
    if ((clientfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        die("socket");
    }
    memset((char * ) & server_socket, 0, sizeof(server_socket));
    server_socket.sin_family = AF_INET;
    server_socket.sin_port = htons(PORT);
    server_socket.sin_addr.s_addr = inet_addr("127.0.0.1");
    pkt.client = 0;

    // initialize file
    FILE* fp = fopen("./input.txt", "r");
    int isEnded = 0;

    while(!isEnded){
        pkt.sequence = 0;
        isEnded = populateBuffer(fp, &pkt);
        transmit(clientfd, &pkt, &server_socket);
    }
    
    close(clientfd);
    fclose(fp);
    return 0;
}
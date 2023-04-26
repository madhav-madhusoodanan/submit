/* Simple udp server with stop and wait functionality */ #include<stdio.h> //printf

#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/time.h>

#define BUFLEN 512 //Max length of buffer
#define PORT 8882 //The port on which to listen for incoming data
#define TIMEOUT 1 // 1 second

char data_1[2048];
char data_2[2048];
char* data[] = {data_1, data_2};

void die(char * s) {
    perror(s);
    exit(1);
}
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

int set_socket(struct sockaddr_in* socket_addr, uint16_t port){
    
    int socket_fd;
    if ((socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
      die("socket");
    }

    memset((char * ) socket_addr, 0, sizeof(struct sockaddr_in));
    socket_addr->sin_family = AF_INET;
    socket_addr->sin_port = htons(port);
    socket_addr->sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socket_fd, (struct sockaddr * ) socket_addr, sizeof(struct sockaddr_in)) != -1){
        return socket_fd;
    } else return -1;
}

void fill(int client, char* packet_data){
    strcat(data[client], packet_data);
}

int main(void) {
    struct sockaddr_in socket_addr_1, socket_addr_2,  si_other;
    int socket_fd, i, slen = sizeof(si_other), recv_len;

    DATA_PKT _data_1;
    DATA_PKT _data_2;
    DATA_PKT _data_temp;
    DATA_PKT data_pkt[] = {_data_1, _data_2, _data_temp};
    ACK_PKT ack_pkt;

    // zero out the structure and get sockets to the connections
    socket_fd = set_socket(&socket_addr_1, PORT);

    bzero(data_1, 2048);
    bzero(data_2, 2048);

    //bind sockets to port
    if (socket_fd < 0) {
      die("bind");
    } else {
        printf("Connected!\n");
    }
    

    while (1) {
            fflush(stdout);
            // try to receive some data, this is a blocking call
            if ((recv_len = recvfrom(socket_fd, &data_pkt[2], BUFLEN, 0, (struct sockaddr * ) &si_other, &slen)) == -1) {
                die("recvfrom()");
            }
            if(data_pkt[data_pkt[2].client].sequence + 1 == data_pkt[2].sequence){
                data_pkt[data_pkt[2].client] = data_pkt[2];
                ack_pkt.sequence = data_pkt[2].sequence;
                ack_pkt.offset = data_pkt[2].offset;
                fill(data_pkt[2].client, data_pkt[2].data);
            }
            printf(" %s\n", data[0]);
            printf(" %s\n", data[1]);
            if (sendto(socket_fd, & ack_pkt, recv_len, 0, (struct sockaddr * ) &si_other, slen) == -1) {
                die("sendto()");
            }
            
    } 
    close(socket_fd);
    // close(socket2fd);
    return 0;
}
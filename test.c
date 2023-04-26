#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<signal.h>

int main(){
    int *process = malloc(sizeof(pid_t));
    *process = 0;
    int pid = fork();

    while(1){
        if(pid == 0){
            sleep(1);
            printf(" %d", *process);
            kill(*process, SIGKILL);
        } else {
            *process = pid;
            printf("Printed!");
            fflush(stdout);
            sleep(100);
        }
    }
    free(process);
}
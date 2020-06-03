#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv) {
    int sockfd = 0;
    char recv_buff[100];
    struct sockaddr_un serv_addr;
    char* socket_path = "./socket";

    memset(recv_buff, '0', sizeof(recv_buff));
    if((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        printf("Error : Could not create socket \n");
        return 1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, socket_path);

    if(connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("Error : Connect Failed \n");
        return 1;
    }

    char msg[100] = "";

    while (1) {
        int readen = read(STDIN_FILENO, msg, sizeof(msg));
        msg[readen] = '\0';
        
        write(sockfd, msg, sizeof(msg));
        read(sockfd, recv_buff, sizeof(recv_buff));
        printf("%s", recv_buff);


    }

}

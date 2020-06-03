#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>


int main(int argc, char **argv) {
    int listenfd, connfd = 0;
    struct sockaddr_in serv_addr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    bind(listenfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));

    listen(listenfd, 10);

    char msg[100];

    connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

    read(connfd, msg, sizeof(msg));
    for (int i = 0; i < strlen(msg); ++i) {
        msg[i] = toupper(msg[i]);
    }

    write(connfd, msg, sizeof(msg));

    close(connfd);
}

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>

int main(int argc, char **argv) {
    int listenfd, connfd = 0;
    struct sockaddr_un serv_addr;
    char* socket_path = "./socket";

    if((listenfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        printf("\nError : Could not create socket \n");
        return 1;
    }
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, socket_path);
    unlink(socket_path);

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

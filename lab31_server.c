#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>
#include <poll.h>

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

    int total_connects = 1;
    struct pollfd fds[11];
    fds[0].fd = listenfd;
    fds[0].events = POLLIN;

    while (1) {
        int count = poll(fds, total_connects, 0);

        if (count == -1) {
            perror("Poll failed");
            return 1;
        }

        if (fds[0].revents & POLLIN) {
            connfd = accept(listenfd, (struct sockaddr *) NULL, NULL);
            fds[total_connects].fd = connfd;
            fds[total_connects].events = POLLIN;
            total_connects++;
        }

        for (int i = 1; i < total_connects; ++i) {
            if (fds[i].revents & POLLIN) {
                if (read(fds[i].fd, msg, sizeof(msg)) == 0) {
                    continue;
                }
                printf("from fd %d: %s", fds[i].fd, msg);
                for (int j = 0; j < strlen(msg); ++j) {
                    msg[j] = toupper(msg[j]);
                }
                write(fds[i].fd, msg, sizeof(msg));
            }
        }
    }
}

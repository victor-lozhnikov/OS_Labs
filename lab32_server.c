#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>
#include <aio.h>

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

    listen(listenfd, 2);

    char bufs[2][100];

    int total_connects = 2;

    struct aiocb **readrqs = (struct aiocb**) malloc(2 * sizeof(struct aiocb*));

    connfd = accept(listenfd, (struct sockaddr *) NULL, NULL);
    struct aiocb *readrq = (struct aiocb*) malloc(sizeof(struct aiocb));
    readrq->aio_fildes = connfd;
    readrq->aio_buf = bufs[0];
    readrq->aio_nbytes = 100;
    readrqs[0] = readrq;

    connfd = accept(listenfd, (struct sockaddr *) NULL, NULL);
    readrq = (struct aiocb*) malloc(sizeof(struct aiocb));
    readrq->aio_fildes = connfd;
    readrq->aio_buf = bufs[0];
    readrq->aio_nbytes = 100;
    readrqs[1] = readrq;

    printf("I'm ready!\n");

    while (1) {
        for (int i = 0; i < total_connects; ++i) {
            aio_read(readrqs[i]);
        }

        aio_suspend((const struct aiocb**)readrqs, total_connects, NULL);

        for (int i = 0; i < total_connects; ++i) {
            int size = aio_return(readrqs[i]);
            if (size > 0) {
                printf("from fd %d: %s", readrqs[i]->aio_fildes, readrqs[i]->aio_buf);
                for (int j = 0; j < strlen((char*)readrqs[i]->aio_buf); ++j) {
                    ((char*)readrqs[i]->aio_buf)[j] = toupper(((char*)readrqs[i]->aio_buf)[j]);
                }
                write(readrqs[i]->aio_fildes, (char*)readrqs[i]->aio_buf, 100);
            }
        }
    }
}

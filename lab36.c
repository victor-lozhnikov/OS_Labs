#include <arpa/inet.h>
#include <assert.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>

int main(int argc, char** argv) {
    char buffer[100000];
    int MAX_REQUEST_LEN = 1024;
    char request[MAX_REQUEST_LEN];
    char request_template[] = "GET / HTTP/1.1\r\nHost: %s\r\n\r\n";
    char *hostname;
    in_addr_t in_addr;
    int request_len;
    int socket_file_descriptor;
    ssize_t nbytes_total, nbytes_last;
    struct hostent *hostent;
    struct sockaddr_in sockaddr_in;
    unsigned short server_port = 80;

    if (argc > 1) {
        hostname = argv[1];
    }
    else {
        fprintf(stderr, "argc < 1\n");
        exit(EXIT_FAILURE);
    }

    request_len = snprintf(request, MAX_REQUEST_LEN, request_template, hostname);
    if (request_len >= MAX_REQUEST_LEN) {
        fprintf(stderr, "request length large: %d\n", request_len);
        exit(EXIT_FAILURE);
    }

    /* Build the socket. */
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /* Build the address. */
    hostent = gethostbyname(hostname);
    if (hostent == NULL) {
        fprintf(stderr, "error: gethostbyname(\"%s\")\n", hostname);
        exit(EXIT_FAILURE);
    }
    in_addr = inet_addr(inet_ntoa(*(struct in_addr*)*(hostent->h_addr_list)));
    if (in_addr == (in_addr_t)-1) {
        fprintf(stderr, "error: inet_addr(\"%s\")\n", *(hostent->h_addr_list));
        exit(EXIT_FAILURE);
    }
    sockaddr_in.sin_addr.s_addr = in_addr;
    sockaddr_in.sin_family = AF_INET;
    sockaddr_in.sin_port = htons(server_port);

    if (connect(socket_file_descriptor, (struct sockaddr*)&sockaddr_in, sizeof(sockaddr_in)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    nbytes_total = 0;
    while (nbytes_total < request_len) {
        nbytes_last = write(socket_file_descriptor, request + nbytes_total, request_len - nbytes_total);
        if (nbytes_last == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
        nbytes_total += nbytes_last;
    }

    struct pollfd fds[2];
    fds[0].fd = socket_file_descriptor;
    fds[0].events = POLLIN;
    fds[1].fd = open("/dev/tty", O_RDONLY);
    fds[1].events = POLLIN;

    int BLOCK_SIZE = 512;

    int nbytes_readen = 0;
    int nbytes_written = 0;
    nbytes_last = read(socket_file_descriptor, buffer, BUFSIZ);
    nbytes_readen += nbytes_last;
    int to_write = (BLOCK_SIZE < nbytes_readen - nbytes_written) ? BLOCK_SIZE : nbytes_readen - nbytes_written;
    write(STDOUT_FILENO, buffer, to_write);
    nbytes_written += to_write;
    printf("\nPress ENTER to continue");
    char* tmp = "\0";

    while (1) {
        int count = poll(fds, 2, 0);
        if (count == -1) {
            perror("Poll failed");
            exit(EXIT_FAILURE);
        }
        if (fds[1].revents & POLLIN) {
            read(fds[1].fd, tmp, BLOCK_SIZE);
            to_write = (BLOCK_SIZE < nbytes_readen - nbytes_written) ? BLOCK_SIZE : nbytes_readen - nbytes_written;
            if (to_write == 0) {
                printf("End!\n");
                break;
            }
            write(STDOUT_FILENO, buffer + nbytes_written, to_write);
            printf("\nPress ENTER to continue");
            nbytes_written += to_write;
        }

        if (fds[0].revents & POLLIN) {
            nbytes_last = read(fds[0].fd, buffer + nbytes_readen, BUFSIZ);
            nbytes_readen += nbytes_last;
        }
    }

    close(socket_file_descriptor);
    exit(EXIT_SUCCESS);
}
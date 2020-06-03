#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv) {

    struct pollfd fds[argc - 1];

    for (int i = 1; i < argc; ++i) {
        if ((fds[i - 1].fd = open(argv[i], O_RDONLY)) == -1) {
            perror("Open error");
            exit(1);
        }
        fds[i - 1].events = POLLIN;
    }

    int count;
    while (1) {
        count = poll(fds, argc - 1, 5000);
        if (count == -1) {
            perror("Poll failed");
            exit(1);
        }
        if (count == 0) {
            printf("No data for reading\n");
            exit(0);
        }
        for (int i = 0; i < argc - 1; ++i) {
            if (fds[i].revents & POLLIN) {
                char out[BUFSIZ];
                int llen = read(fds[i].fd, out, BUFSIZ - 1);
                if (llen == 0) {
                    fds[i].events = 0;
                    continue;
                }
                out[llen] = '\0';
                printf("Data in fd %d:\n%s\n", fds[i].fd, out);
            }
        }
    }
}

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char **argv) {
    int fd[2];
    pid_t pid;

    if (argc == 1) {
        return 0;
    }

    char msgout[100] = "";
    for (int i = 1; i < argc; ++i) {
        strcat(msgout, argv[i]);
        strcat(msgout, " ");
    }

    size_t msg_size = strlen(msgout);
    char msgin[msg_size];

    if (pipe(fd) == -1) {
        perror(argv[0]);
        exit(1);
    }

    if ((pid = fork()) > 0) {
        write(fd[1], msgout, msg_size);
    }
    else if (pid == 0) {
        read(fd[0], msgin, msg_size);
        for (int i = 0; i < msg_size; ++i) {
            if (msgin[i] == '\0') {
                break;
            }
            printf("%c", toupper(msgin[i]));
        }
        printf("\n");
    }
    else {
        perror(argv[0]);
        exit(2);
    }
    return 0;
}

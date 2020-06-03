#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>

void *thread (void *param);

int main (int argc, char **argv) {
    if (argc == 1) {
        return 0;
    }

    char msg[100] = "";

    for (int i = 1; i < argc; ++i) {
        strcat(msg, argv[i]);
        strcat(msg, " ");
    }

    pthread_t tid;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, thread, &msg);

    pthread_join(tid, NULL);

    size_t msg_size = strlen(msg);
    for (int i = 0; i < msg_size; ++i) {
        printf("%c", msg[i]);
    }
    printf("\n");
}

void *thread (void *param) {
    char* msg = (char*) param;
    size_t msg_size = strlen(msg);
    for (int i = 0; i < msg_size; ++i) {
        msg[i] = toupper(msg[i]);
    }
    pthread_exit(0);
}
#include <stdio.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <zconf.h>

int err_count = 0;
int inc_count = 0;

void sig_quit (int sig) {
    printf("\nSIGINT: %d\nSIGALARM: %d\n", err_count, inc_count);
    exit(0);
}

void sig_int (int sig) {
    err_count++;
    printf("\7\n");
}

void sig_inc (int sig) {
    inc_count++;
    alarm(1);
}

int main() {
    alarm(1);
    while (true) {
        signal(SIGQUIT, sig_quit);
        signal(SIGINT, sig_int);
        signal(SIGALRM, sig_inc);
    }
}

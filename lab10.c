#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
    char *new_args[argc];
    for (int i = 0; i < argc - 1; ++i) {
        new_args[i] = argv[i + 1];
    }
    new_args[argc - 1] = (char*) 0;

    execv(argv[1], new_args);
    return 0;
}

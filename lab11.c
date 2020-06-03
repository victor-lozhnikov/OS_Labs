#include <stdio.h>
#include <zconf.h>

extern char** environ;

int execvpe(const char *filename, char *const argv[], char *envp[]) {
    environ = &envp[0];
    return execvp(filename, argv);
}

int main(int argc, char **argv) {
    char *nargv[] = {
            "lab1", "-u", "-v", "-i", (char*) 0
    };
    char *nenvp[] = {
            "name=value", "123=456", "home=/kek", "abc=ivan", (char*) 0
    };

    execvpe("lab1", nargv, nenvp);
    return 0;
}

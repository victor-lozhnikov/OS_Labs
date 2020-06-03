#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <wait.h>
#include <string.h>
 
int main() {
    FILE* file;
    pid_t pid;
 
    if ((pid = fork()) > 0) {
        file = fopen("text1", "r");
    }
    else if (pid == 0) {
        file = fopen("text2", "r");
    }
    else {
        exit(1);
    }
 
    char *str = (char*)malloc(1000 * sizeof(char));
    char *to_print = (char*)malloc(1000 * sizeof(char));
    size_t len = 0;
 
    if (getline(&str, &len, file) == -1) {
        wait(NULL);
        printf("%s\n", str);
        exit(0);
    }
 
    strcpy(to_print, str);
 
    while (getline(&str, &len, file) != -1) {
        printf("%s", to_print);
        strcpy(to_print, str);
    }
 
    wait(NULL);
 
    printf("%s\n", to_print);
    return 0;
}
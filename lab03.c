#include <stdio.h>
#include <unistd.h>

int main() {
    printf("     Real ID:\t%d\n", getuid());
    printf("Effective ID:\t%d\n", geteuid());

    FILE *file;
    if ((file = fopen("file", "r")) == NULL) {
        perror("Can't open file");
    }
    else {
        printf("File opened!\n");
    }

    printf("\n");
    seteuid(getuid());

    printf("     Real ID:\t%d\n", getuid());
    printf("Effective ID:\t%d\n", geteuid());

    if ((file = fopen("file", "r")) == NULL) {
        perror("Can't open file");
    }
    else {
        printf("File opened!\n");
    }

    return 0;
}

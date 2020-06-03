#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


int main(int argc, char **argv) {
    char msgout[100] = "wc ";
    for (int i = 1; i < argc; ++i) {
        strcat(msgout, argv[i]);
        strcat(msgout, " ");
    }
    char line[100];

    FILE *fp;
    fp = popen(msgout, "r");

    while (fgets(line, BUFSIZ, fp) != (char *)NULL) {
        printf("%s", line);
    }

    pclose(fp);

    return 0;
}

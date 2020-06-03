#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>

bool check(char *s, char *p) {
    char *rs = 0, *rp = 0;
    while(1) {
        if (*p == '*') {
            rs = s;
            p++;
            rp = p;
        }
        else if (*s == '\0') {
            return (*p == '\0');
        }
        else if (*s == *p || *p == '?') {
            s++;
            p++;
        }
        else if (rs) {
            rs++;
            s = rs;
            p = rp;
        }
        else
            return false;
    }
}

int main() {
    DIR *dirp;
    struct dirent *dp;
    struct stat *stbuf;
    char pattern[256];

    scanf("%s", pattern);

    if ((dirp = opendir(".")) == NULL) {
        exit(1);
    }

    while ((dp = readdir(dirp)) != NULL) {
        if (check(dp->d_name, pattern)) {
            printf("%s\n", dp->d_name);
        }
    }
    return 0;
}

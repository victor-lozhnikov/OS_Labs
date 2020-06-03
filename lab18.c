#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>

int main(int argc, char **argv) {
    struct stat stbuf;

    for (int i = 1; i < argc; ++i) {
        if (stat(argv[i], &stbuf) == -1) {
            printf("Can't find file \"%s\"\n", argv[i]);
            continue;
        }
        switch (stbuf.st_mode & S_IFMT) {
            case S_IFDIR:
                printf("d  ");
                break;
            case S_IFREG:
                printf("-  ");
                break;
            case S_IFCHR:
                printf("c  ");
                break;
            case S_IFBLK:
                printf("b  ");
                break;
            case S_IFLNK:
                printf("l  ");
                break;
            case S_IFSOCK:
                printf("s  ");
                break;
            default:
                printf("?  ");
                break;
        }

        char perm_mask[9] = "xwrxwrxwr";
        int perm = stbuf.st_mode & 0777;

        for (int j = 8; j >= 0; --j) {
            if ((perm >> j) & 1) {
                printf("%c", perm_mask[j]);
            }
            else {
                printf("-");
            }
        }
        printf("  ");

        printf("%3d  ", stbuf.st_nlink);

        struct passwd *pw;
        struct group *grp;

        pw = getpwuid(stbuf.st_uid);
        grp = getgrgid(stbuf.st_gid);

        printf("%10s  %10s  ", pw->pw_name, grp->gr_name);

        if ((stbuf.st_mode & S_IFMT) != S_IFDIR) {
            printf("%9ld  ", stbuf.st_size);
        }
        else printf ("           ");

        printf("%.24s  ", ctime(&stbuf.st_mtime));

        int last_slash = 0;
        for (int j = 0; j < strlen(argv[i]); ++j) {
            if (argv[i][j] == '/') {
                last_slash = j + 1;
            }
        }

        printf("%s\n",  (argv[i] + last_slash));
    }

    return 0;
}

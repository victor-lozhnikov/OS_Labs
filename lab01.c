#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <ulimit.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;

int main(int argc, char **argv, char **envp) {
    const char options[] = "ispuU:cC:dvV:";

    if (argc == 1) {
        printf("Options not found\n");
        return 0;
    }

    int res = 0;
    long new_ulimit;
    long new_climit;
    struct rlimit rlim;
    char buf[1000];

    while ((res = getopt(argc, argv, options)) != -1) {
        switch (res) {
            case 'i':
                printf("Real UID:\t%d\n", getuid());
                printf("Effective UID:\t%d\n", geteuid());
                printf("Real GID:\t%d\n", getgid());
                printf("Effective GID:\t%d\n\n", getegid());
                break;
            case 's':
                setpgid(0, 0);
                break;
            case 'p':
                printf("PID:\t%d\n", getpid());
                printf("PPID:\t%d\n", getppid());
                printf("PGID:\t%d\n\n", getpgid(0));
                break;
            case 'u':
                printf("ulimit:\t%ld\n\n", ulimit(UL_GETFSIZE));
                break;
            case 'U':
                printf("%s\n", optarg);
                new_ulimit = atol(optarg);
                ulimit(UL_SETFSIZE, new_ulimit);
                break;
            case 'c':
                getrlimit(RLIMIT_CORE, &rlim);
                printf("сur RLIMIT_CORE:\t%ld\n", rlim.rlim_cur);
                printf("max RLIMIT_CORE:\t%ld\n\n", rlim.rlim_max);
                break;
            case 'C':
                new_climit = atol(optarg);
                rlim.rlim_cur = new_climit;
                rlim.rlim_max = new_climit;
                setrlimit(RLIMIT_CORE, &rlim);
                break;
            case 'd':
                getcwd(buf, 1000);
                printf("%s\n\n", buf);
                break;
            case 'v':
                for (char** i = environ; *i != NULL; ++i) {
                    printf("%s\n", *i);
                }
                printf("\n");
                break;
            case 'V':
                putenv(optarg);
                break;
            default:
                printf("Command not found\n\n");
                break;
        }
    }

    return 0;
}

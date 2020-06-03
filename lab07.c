#include <stdio.h>
#include <fcntl.h>
#include <zconf.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/mman.h>

int file;
char* buf;

void print_file (int sig) {
    for (int i = 0; *(buf + i) != '\0'; ++i) {
        printf("%c", *(buf + i));
    }
    exit(0);
}

int time_scan (int sec, char* input) {
   signal(SIGALRM, print_file);
   alarm(sec);
   int res = scanf("%s", input);
   alarm(0);
   return res;
}

int main() {
    if ((file = open("crusoe", O_RDONLY)) == -1) {
        perror("Can't open file");
        return 1;
    }

    int str_cnt = 0;

    int off = 0;
    int count_of_pages = 0;

    int end_of_file = 0;
    while (!end_of_file) {
        char* c = (char*) mmap(0, getpagesize(), PROT_READ, MAP_PRIVATE, file, off);
        count_of_pages++;
        for (int i = 0; i < getpagesize(); ++i) {
            if (*(c + i) == '\0') {
                end_of_file = 1;
                break;
            }
            if (*(c + i) == '\n') {
                str_cnt++;
            }
            //printf("%c", *(c + i));
        }
        off += getpagesize();
    }

    int break_indexes[str_cnt + 1];
    int str_lens[str_cnt];
    break_indexes[0] = -1;

    printf("%d\n", str_cnt);

    buf = (char*) mmap(0, count_of_pages * getpagesize(), PROT_READ, MAP_PRIVATE, file, 0);

    int cur_str = 1;

    for (int i = 0; *(buf + i) != '\0'; ++i) {
        if (*(buf + i) == '\n') {
            break_indexes[cur_str] = i;
            str_lens[cur_str - 1] = break_indexes[cur_str] - break_indexes[cur_str - 1] - 1;
            cur_str++;
        }
    }

    char* end;
    char* input = (char*) malloc(30 * sizeof(char));

    int res = time_scan(5, input);

    if (res == EOF) {
        printf("Don't press Ctrl+D, mudila!\n");
        exit(1);
    }

    int query = strtol(input, &end, 10);

    while (query != 0 || end == input) {
        if (query < 0 || query > str_cnt || end == input) {
            fprintf(stderr, "Wrong input\n");
            int res = time_scan(5, input);
            if (res == EOF) {
                printf("Don't press Ctrl+D, mudila!\n");
                exit(1);
            }
            query = strtol(input, &end, 10);
            continue;
        }

        query--;

        for (int i = 0; i < str_lens[query]; ++i) {
            printf("%c", *(buf +  break_indexes[query] + i + 1));
        }
        printf("\n");

        int res = time_scan(5, input);

        if (res == EOF) {
            printf("Don't press Ctrl+D, mudila!\n");
            exit(1);
        }

        query = strtol(input, &end, 10);
    }

    close(file);
    return 0;
}

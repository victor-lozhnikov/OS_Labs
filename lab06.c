#include <stdio.h>
#include <fcntl.h>
#include <zconf.h>
#include <stdlib.h>
#include <signal.h>
#include <ctype.h>

int file;

void print_file (int sig) {
    char* c = (char*) malloc(sizeof(char));
    lseek(file, 0, 0);
    while (read(file, c, 1) != 0) {
        printf("%c", *c);
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

    char* c = (char*) malloc(sizeof(char));

    while (read(file, c, 1) != 0) {
        if (*c == '\n') {
            str_cnt++;
        }
    }
    printf("%d\n", str_cnt);

    int break_indexes[str_cnt + 1];
    int str_lens[str_cnt];
    break_indexes[0] = -1;

    lseek(file, 0, 0);

    int cur_str = 1;
    int cur_pos = 0;

    while (read(file, c, 1) != 0) {
        if (*c == '\n') {
            break_indexes[cur_str] = cur_pos;
            str_lens[cur_str - 1] = break_indexes[cur_str] - break_indexes[cur_str - 1] - 1;
            cur_str++;
        }
        cur_pos++;
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
        lseek(file, break_indexes[query] + 1, 0);

        char* buf = malloc(str_lens[query] * sizeof(char) + 1);
        char* rev_buf = malloc(str_lens[query] * sizeof(char) + 1);
        read(file, buf, str_lens[query]);
        *(buf + str_lens[query]) = '\0';

        int i_r = 0;
        for (int i = str_lens[query] - 1; i >= 0; --i) {
            rev_buf[i_r] = buf[i];
            i_r++;
        }

        rev_buf[0] = toupper(rev_buf[0]);
        rev_buf[i_r - 1] = tolower(rev_buf[i_r - 1]);
        rev_buf[i_r] = '\0';

        printf("%s\n", rev_buf);
        free(buf);
        free(rev_buf);

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

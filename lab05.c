#include <stdio.h>
#include <fcntl.h>
#include <zconf.h>
#include <malloc.h>
#include <stdlib.h>
#include <errno.h>

int main() {
    int file;
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

    /*for (int i = 0; i < str_cnt; ++i) {
        printf("%d ", str_lens[i]);
    }
    printf("\n");*/

    char* end;
    char* input = (char*) malloc(30 * sizeof(char));

    if (scanf("%s", input) == -1) {
        exit(1);
    }

    int query = strtol(input, &end, 10);

    while (query != 0 || end == input) {
        if (query < 0 || query > str_cnt || end == input) {
            fprintf(stderr, "Wrong input\n");
            scanf("%s", input);
            query = strtol(input, &end, 10);
            continue;
        }

        query--;
        lseek(file, break_indexes[query] + 1, 0);

        char* buf = malloc(str_lens[query] * sizeof(char) + 1);
        read(file, buf, str_lens[query]);
        *(buf + str_lens[query]) = '\0';
        printf("%s\n", buf);
        free(buf);

        if (scanf("%s", input) == -1) {
            exit(1);
        }
        query = strtol(input, &end, 10);
    }

    close(file);
    return 0;
}

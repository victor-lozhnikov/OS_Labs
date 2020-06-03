#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <libgen.h>

int main() {
    srand(time(NULL));

    FILE *fptrs[2];

    char lines[100][3];

    for (int i = 0; i < 100; ++i) {
        lines[i][0] = (char) (rand() % 100 + 1);
        lines[i][1] = '\n';
        lines[i][2] = '\0';
    }

    p2open("sort ", fptrs);

    for (int i = 0; i < 100; ++i) {
        fputs(lines[i], fptrs[0]);
    }
    fclose(fptrs[0]);

    char nums[10][10];

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            char tmp;
            fscanf(fptrs[1], "%c", &nums[i][j]);
            fscanf(fptrs[1], "%c", &tmp);
            printf("%d ", nums[i][j] - 1);
        }
        printf("\n");
    }
    fclose(fptrs[1]);
    p2close(fptrs);

    return 0;
}

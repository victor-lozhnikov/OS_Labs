#include <stdio.h>
#include <termio.h>

int main() {
    printf ("Press any key\n");

    struct termios stored_settings;
    struct termios new_settings;

    tcgetattr(fileno(stdin), &stored_settings);

    new_settings = stored_settings;

    new_settings.c_lflag &= (~ICANON);
    new_settings.c_cc[VMIN] = 1;

    tcsetattr(fileno(stdin), TCSAFLUSH, &new_settings);

    int a[10] = {3, 4, 5, 7, 8, 9, 11, 12, 14, 15};
    char template[16] = "+7(___)___-__-__";

    char cur_c;
    for (int i = 0; i < 10; ++i) {
        scanf("%c", &cur_c);
        template[a[i]] = cur_c;
        printf("\n%s\n", template);
    }

    tcsetattr(fileno(stdin), TCSAFLUSH, &stored_settings);

}

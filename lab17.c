#include <stdio.h>
#include <termio.h>

int is_printed(char c) {
    if (c < 32 || c == 127) {
        return 0;
    }
    return 1;
}

void backspace (int cur_pos) {
    switch (cur_pos) {
        case 0:
            break;
        case 1:
            printf("\b \b\b \b\b \b\b \b");
            break;
        case 4:
            printf("\b \b\b \b\b \b");
            break;
        case 7:
        case 9:
            printf("\b \b\b \b");
            break;
        default:
            printf("\b \b");
    }
}

int main() {
    struct termios stored_settings;
    struct termios new_settings;

    char tmp;

    tcgetattr(fileno(stdin), &stored_settings);

    new_settings = stored_settings;

    new_settings.c_lflag &= ~(ICANON | ECHO);
    new_settings.c_cc[VMIN] = 1;

    tcsetattr(fileno(stdin), TCSAFLUSH, &new_settings);

    int MAXLEN = 10;
    int cur_pos = 0;
    char cur_str[MAXLEN];

    while(1) {
        scanf("%c", &tmp);
        //printf("%d ", tmp);
        if (tmp == 4 && cur_pos == 0) {
            break;
        }
        else if (tmp == 10) {
            printf("\n");
            cur_pos = 0;
        }
        else if (tmp == 127) {
            backspace(cur_pos);
            if (cur_pos > 0)
                cur_pos--;
        }
        else if (tmp == 8) {
            int k = cur_pos;
            for (int i = 0; i < k; ++i) {
                backspace(cur_pos);
                cur_pos--;
            }
        }
        else if (tmp == 23) {
            while (cur_pos > 0 && cur_str[cur_pos - 1] == ' ') {
                backspace(cur_pos);
                cur_pos--;
            }
            while (cur_pos > 0 && cur_str[cur_pos - 1] != ' ') {
                backspace(cur_pos);
                cur_pos--;
            }
        }
        else if (is_printed(tmp) == 1) {
            if (cur_pos == MAXLEN) {
                printf("\n");
                cur_pos = 0;
            }
            switch (cur_pos) {
                case 0:
                    printf("+7(%c", tmp);
                    break;
                case 3:
                    printf(")-%c", tmp);
                    break;
                case 6:
                case 8:
                    printf("-%c", tmp);
                    break;
                default:
                    printf("%c", tmp);
                    break;
            }
            cur_str[cur_pos] = tmp;
            cur_pos++;
        }
        else {
            printf("\a");
        }
    }

    tcsetattr(fileno(stdin), TCSAFLUSH, &stored_settings);

}

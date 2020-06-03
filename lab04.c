#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int BLOCK_SIZE = 2;

typedef struct node node;

struct node {
    node* next;
    char* str;
};

int main() {
    char buf[BLOCK_SIZE];

    node* root = (node*) malloc(sizeof(node));
    if (root == NULL) {
        exit(1);
    }

    root->next = NULL;
    node* cur = root;

    fgets(buf, BLOCK_SIZE, stdin);
    while (buf[0] != '.') {
        int len = strlen(buf) + 1;
        int cur_size = 0;

        node* next = (node*) malloc(sizeof(node));
        if (next == NULL)  {
            exit(1);
        }
        cur->next = next;

        char* cur_str = (char*) malloc(sizeof(char));
        *cur_str = '\0';

        while (buf[len - 2] != '\n') {
            cur_str = (char*) realloc(cur_str, (cur_size + len) * sizeof(char));
            strcat(cur_str, buf);
            cur_size = strlen(cur_str) + 1;
            fgets(buf, BLOCK_SIZE, stdin);
            len = strlen(buf) + 1;
        }

        cur_str = (char*) realloc(cur_str, (cur_size + len) * sizeof(char));
        strcat(cur_str, buf);

        next->str = cur_str;
        next->next = NULL;
        cur = next;

        fgets(buf, BLOCK_SIZE, stdin);
    }

    cur = root->next;

    while (cur != NULL) {
        printf("%s", cur->str);
        cur = cur->next;
    }
}
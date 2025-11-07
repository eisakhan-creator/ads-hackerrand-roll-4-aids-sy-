#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readline();
char* ltrim(char*);
char* rtrim(char*);

int parse_int(char*);

/*
 * Complete the 'twoTwo' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts STRING a as parameter.
 */

int twoTwo(char* a) {
        // Trie node definition
    typedef struct Node {
        bool end;
        struct Node* next[10];
    } Node;

    Node* newNode() {
        Node* node = (Node*)calloc(1, sizeof(Node));
        return node;
    }

    void insert(Node* root, const char* s) {
        Node* curr = root;
        for (int i = 0; s[i]; i++) {
            int idx = s[i] - '0';
            if (!curr->next[idx])
                curr->next[idx] = newNode();
            curr = curr->next[idx];
        }
        curr->end = true;
    }

    char* multiplyBy2(const char* num) {
        int len = strlen(num);
        char* res = (char*)calloc(len + 5, sizeof(char));
        int carry = 0;
        for (int i = len - 1; i >= 0; i--) {
            int val = (num[i] - '0') * 2 + carry;
            res[i + 1] = (val % 10) + '0';
            carry = val / 10;
        }
        if (carry) {
            res[0] = carry + '0';
            return res;
        } else {
            char* out = strdup(res + 1);
            free(res);
            return out;
        }
    }

    Node* root = newNode();
    char* num = strdup("1");
    for (int i = 0; i <= 800; i++) {
        insert(root, num);
        char* temp = multiplyBy2(num);
        free(num);
        num = temp;
    }
    free(num);

    int n = strlen(a);
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (a[i] == '0') continue;
        Node* curr = root;
        for (int j = i; j < n; j++) {
            int idx = a[j] - '0';
            if (!curr->next[idx]) break;
            curr = curr->next[idx];
            if (curr->end)
                count++;
        }
    }

    void freeTrie(Node* node) {
        if (!node) return;
        for (int i = 0; i < 10; i++)
            freeTrie(node->next[i]);
        free(node);
    }

    freeTrie(root);
    return count;

    

}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    int t = parse_int(ltrim(rtrim(readline())));

    for (int t_itr = 0; t_itr < t; t_itr++) {
        char* a = readline();

        int result = twoTwo(a);

        fprintf(fptr, "%d\n", result);
    }

    fclose(fptr);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;

    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!data) {
            data = '\0';

            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);

        if (!data) {
            data = '\0';
        }
    } else {
        data = realloc(data, data_length + 1);

        if (!data) {
            data = '\0';
        } else {
            data[data_length] = '\0';
        }
    }

    return data;
}

char* ltrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    while (*str != '\0' && isspace(*str)) {
        str++;
    }

    return str;
}

char* rtrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    char* end = str + strlen(str) - 1;

    while (end >= str && isspace(*end)) {
        end--;
    }

    *(end + 1) = '\0';

    return str;
}

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);

    if (endptr == str || *endptr != '\0') {
        exit(EXIT_FAILURE);
    }

    return value;
}

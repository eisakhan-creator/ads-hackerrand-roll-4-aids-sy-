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
char** split_string(char*);

int parse_int(char*);

/*
 * Complete the 'matrixRotation' function below.
 *
 * The function accepts following parameters:
 *  1. 2D_INTEGER_ARRAY matrix
 *  2. INTEGER r
 */

void matrixRotation(int matrix_rows, int matrix_columns, int** matrix, int r) {
        int layers = (matrix_rows < matrix_columns ? matrix_rows : matrix_columns) / 2;

    for (int layer = 0; layer < layers; layer++) {
        int top = layer;
        int left = layer;
        int bottom = matrix_rows - layer - 1;
        int right = matrix_columns - layer - 1;

        int size = 2 * (bottom - top + right - left);
        int *arr = (int*)malloc(size * sizeof(int));
        int idx = 0;

        for (int j = left; j <= right; j++)
            arr[idx++] = matrix[top][j];
        for (int i = top + 1; i <= bottom; i++)
            arr[idx++] = matrix[i][right];
        for (int j = right - 1; j >= left; j--)
            arr[idx++] = matrix[bottom][j];
        for (int i = bottom - 1; i > top; i--)
            arr[idx++] = matrix[i][left];

        int rot = r % size;
        int *rotArr = (int*)malloc(size * sizeof(int));
        for (int i = 0; i < size; i++)
            rotArr[i] = arr[(i + rot) % size];

        idx = 0;
        for (int j = left; j <= right; j++)
            matrix[top][j] = rotArr[idx++];
        for (int i = top + 1; i <= bottom; i++)
            matrix[i][right] = rotArr[idx++];
        for (int j = right - 1; j >= left; j--)
            matrix[bottom][j] = rotArr[idx++];
        for (int i = bottom - 1; i > top; i--)
            matrix[i][left] = rotArr[idx++];

        free(arr);
        free(rotArr);
    }

    for (int i = 0; i < matrix_rows; i++) {
        for (int j = 0; j < matrix_columns; j++) {
            printf("%d", matrix[i][j]);
            if (j < matrix_columns - 1) printf(" ");
        }
        printf("\n");
    }


}

int main()
{
    char** first_multiple_input = split_string(rtrim(readline()));

    int m = parse_int(*(first_multiple_input + 0));

    int n = parse_int(*(first_multiple_input + 1));

    int r = parse_int(*(first_multiple_input + 2));

    int** matrix = malloc(m * sizeof(int*));

    for (int i = 0; i < m; i++) {
        *(matrix + i) = malloc(n * (sizeof(int)));

        char** matrix_item_temp = split_string(rtrim(readline()));

        for (int j = 0; j < n; j++) {
            int matrix_item = parse_int(*(matrix_item_temp + j));

            *(*(matrix + i) + j) = matrix_item;
        }
    }

    matrixRotation(m, n, matrix, r);

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

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);

        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }

    return splits;
}

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);

    if (endptr == str || *endptr != '\0') {
        exit(EXIT_FAILURE);
    }

    return value;
}

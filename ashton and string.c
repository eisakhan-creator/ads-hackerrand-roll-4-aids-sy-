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
 * Complete the 'ashtonString' function below.
 *
 * The function is expected to return a CHARACTER.
 * The function accepts following parameters:
 *  1. STRING s
 *  2. INTEGER k
 */

char ashtonString(char* s, int k) {
        int n = strlen(s);

    // ----- Build Suffix Array -----
    typedef struct {
        int index;
        int rank[2];
    } Suffix;

    Suffix *suffixes = malloc(n * sizeof(Suffix));
    int *index = malloc(n * sizeof(int));
    int *suffixArr = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        suffixes[i].index = i;
        suffixes[i].rank[0] = s[i] - 'a';
        suffixes[i].rank[1] = (i + 1 < n) ? (s[i + 1] - 'a') : -1;
    }

    int cmpSuffix(const void *a, const void *b) {
        const Suffix *x = (const Suffix *)a;
        const Suffix *y = (const Suffix *)b;
        if (x->rank[0] == y->rank[0])
            return x->rank[1] - y->rank[1];
        return x->rank[0] - y->rank[0];
    }

    qsort(suffixes, n, sizeof(Suffix), cmpSuffix);

    for (int k2 = 4; k2 < 2 * n; k2 <<= 1) {
        int rank = 0;
        int prev_rank = suffixes[0].rank[0];
        suffixes[0].rank[0] = 0;
        index[suffixes[0].index] = 0;

        for (int i = 1; i < n; i++) {
            if (suffixes[i].rank[0] == prev_rank &&
                suffixes[i].rank[1] == suffixes[i - 1].rank[1]) {
                prev_rank = suffixes[i].rank[0];
                suffixes[i].rank[0] = rank;
            } else {
                prev_rank = suffixes[i].rank[0];
                suffixes[i].rank[0] = ++rank;
            }
            index[suffixes[i].index] = i;
        }

        for (int i = 0; i < n; i++) {
            int nextIndex = suffixes[i].index + k2 / 2;
            suffixes[i].rank[1] =
                (nextIndex < n) ? suffixes[index[nextIndex]].rank[0] : -1;
        }

        qsort(suffixes, n, sizeof(Suffix), cmpSuffix);
    }

    for (int i = 0; i < n; i++)
        suffixArr[i] = suffixes[i].index;

    free(index);
    free(suffixes);

    // ----- Build LCP Array -----
    int *lcp = calloc(n, sizeof(int));
    int *inv = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        inv[suffixArr[i]] = i;

    int len = 0;
    for (int i = 0; i < n; i++) {
        if (inv[i] == n - 1) {
            len = 0;
            continue;
        }
        int j = suffixArr[inv[i] + 1];
        while (i + len < n && j + len < n && s[i + len] == s[j + len])
            len++;
        lcp[inv[i]] = len;
        if (len > 0) len--;
    }
    free(inv);

    // ----- Find k-th character -----
    long long total = 0;
    char ans = '?';

    for (int i = 0; i < n; i++) {
        int start = suffixArr[i];
        int common = (i == 0) ? 0 : lcp[i - 1];
        int suffixLen = n - start;

        for (int l = common + 1; l <= suffixLen; l++) {
            if (total + l < k) {
                total += l;
            } else {
                int pos = (int)(k - total - 1);
                ans = s[start + pos];
                free(suffixArr);
                free(lcp);
                return ans;
            }
        }
    }

    free(suffixArr);
    free(lcp);
    return ans;


}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    int t = parse_int(ltrim(rtrim(readline())));

    for (int t_itr = 0; t_itr < t; t_itr++) {
        char* s = readline();

        int k = parse_int(ltrim(rtrim(readline())));

        char res = ashtonString(s, k);

        fprintf(fptr, "%c\n", res);
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

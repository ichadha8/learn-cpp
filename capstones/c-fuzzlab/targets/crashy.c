#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        return 2;
    }

    FILE *fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        return 3;
    }

    char buffer[256];
    size_t n = fread(buffer, 1, sizeof buffer - 1, fp);
    fclose(fp);
    buffer[n] = '\0';

    if (strstr(buffer, "CRASH") != NULL) {
        volatile int *p = NULL;
        *p = 1;
    }

    if (strstr(buffer, "HANG") != NULL) {
        for (;;) {
        }
    }

    return 0;
}

#include <stdio.h>

int main(int argc, char **argv) {
    // printf("%d %s\n", argc, argv[0]);
    for (int i = 0; i < argc; i++) {
        printf("%d %s\n", i, argv[i]);
    }

    printf("%d\n", argv)
}
#include <stdio.h>
#include "utils.h"

int get_int_input(const char* prompt) {
    int value;
    printf("%s", prompt);
    scanf("%d", &value);
    return value;
}

void flush_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

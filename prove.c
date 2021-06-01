#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXCOMMAND 5000

int main(void) {

    char *command = malloc(MAXCOMMAND), *ptr;
    int d, k;

    command = fgets(command, MAXCOMMAND, stdin);
    //printf("%s", command);

    int space_pos = 0;
    while (command[space_pos] != ' ') space_pos++; // space_pos is at the position of the space_pos (starting from 0)

    char *num1 = malloc(space_pos);
    char *num2 = malloc(10);

    num1 = strncpy(num1, command, space_pos);
    num2 = strncpy(num2, command + space_pos + 1, strlen(command) - space_pos - 2);

    printf("%s --- %s\n", num1, num2);

    d = strtol(num1, &ptr, 10);
    k = strtol(num2, &ptr, 10);

    printf("%d -- %d\n", d, k);
    printf("%s", ptr);

}
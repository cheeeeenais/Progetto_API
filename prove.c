#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void) {

    char *command = malloc(5000);
    //printf("%llu\n", sizeof(command));

    fgets(command, 5000, stdin);
    printf("%s\n%llu", command, strlen(command));
    /*while (command[0] != 'T') {
        fgets(command, sizeof(command), stdin);
        printf("%s", command);
    }*/
}
//
// Created by chen on 27/05/2021.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {

    char first[500], *num1 = NULL, *num2 = NULL, command[500];
    int d, k;

    fgets(first, 500, stdin);
    //memset(num1, 0, 50);
    //memset(num2, 0, 50);

    int space_pos = 0;
    while (first[space_pos] != ' ') {
        space_pos++;
    };

    // space_pos is at the position of the space_pos
    // first[space_pos] is the space_pos
    printf("%d", space_pos);
    printf("\n%c", first[space_pos]);

    // TODO fix
    //d = atoi(strncpy(num1, first, space_pos - 1));
    //k = atoi(strncpy(num2, first + space_pos, strlen(first) - space_pos - 2));

    fgets(command, 500, stdin);
    while (command[0] != 'T') {
        unsigned long weight;
        printf("\nciao");
        fgets(command, 500, stdin);
    }
}
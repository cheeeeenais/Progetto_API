//
// Created by chen on 27/05/2021.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// skip the first number
/*unsigned long secondNumberFinder(char *command) {
    int firstComma, secondComma, i = 0;
    while (command[i] != ',') i++;
    firstComma = i;
    //printf("%d\n", firstComma);
    i++;
    while (command[i] != ',') i++;
    secondComma = i;
    //printf("%d\n", secondComma);
    char *dest = malloc(sizeof(unsigned long));
    strncpy(dest, command + firstComma + 1, secondComma - firstComma - 1);
    //printf("%s\n", dest);
    return _atoi64(dest);
}*/

/**
 * @param command
 * @param index of the first number to begin the search with
 * @return the number between two commas or the last number
 */
unsigned long weightParser(char *command, int index) {
    if (command[index] == '0') return 0;
    int i = 1;
    while (command[index + i] != ',') i++;
    char *dest = malloc(sizeof(unsigned long));
    return _atoi64(strncpy(dest, command + index, i));
}

int main(void) {

    char *command = malloc(sizeof(unsigned long int) * 2);
    int d, k;

    // get the first line (with d and k)
    fgets(command, 100, stdin);

    int space_pos = 0;
    while (command[space_pos] != ' ') space_pos++; // space_pos is at the position of the space_pos (starting from 0)

    char *num1 = malloc(space_pos);
    char *num2 = malloc(500);

    d = atoi(strncpy(num1, command, space_pos));
    k = atoi(strncpy(num2, command + space_pos + 1, strlen(command) - space_pos - 2));

    free(num1);
    free(num2);

    unsigned long *graphVector = malloc(sizeof(d - 1) * sizeof(unsigned long));
    unsigned long *winnersVector = malloc(sizeof(k) * sizeof(unsigned long));
    int winnerIndex = 0;


    //fgets(command, 16, stdin); // to read the first line, which is "AggiungiGrafo"
    while (fgets(command, 100, stdin) != NULL) {

        if (command[0] == 'A') {

            int index = 1;

            // I have to take only d-1 numbers (d except the first one)
            for (int d_count = 0; d_count < d; d_count++) {
                int comma_count = 1;
                while (comma_count < d) {
                    while (command[index] != ',') index++;
                    unsigned long weight = weightParser(command, index + 1); // I pass the command & index of the number after the comma
                    //printf("%lu\n", weight);

                    int graphVectorIndex = 0;

                    if (weight > 0 && weight > graphVector[index]) {
                        graphVector[index] = weight;
                    }

                    comma_count++;
                    graphVectorIndex++; // these two are the same thing
                }
            }

            // calculate the sum of the graphVector and assign it to the winnersVector
            // TODO think of an algorithm
            unsigned long sum = 0;
            for (int i = 0; i < d - 1; ++i) {
                sum = sum + graphVector[i];
            }
            if (sum < winnersVector[winnerIndex]) {
                winnersVector[winnerIndex + 1] = winnersVector[winnerIndex];
                winnersVector[winnerIndex] = sum;
            }
            else winnersVector[winnerIndex + 1] = sum;

        }
        else if (command[0] != 'T') {
            for (int i = 0; i < k; ++i) {}
        }


    }

}

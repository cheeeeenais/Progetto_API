//
// Created by chen on 27/05/2021.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct node{
    unsigned long sum;
    int graph_index;
    struct node *left;
    struct node *right;
    struct node *father;
} WinnerNode;

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

void printTree(WinnerNode *node, WinnerNode *max) {
    if (node == NULL) return;

    printTree(node->left, max);
    printf("%d", node->graph_index);
    if (node != max) printf(" ");
    else printf("\n");
    printTree(node->right, max);
}

/**
 * @param command
 * @param index of the first number to begin the search with
 * @return the number between two commas or the last number
 */
unsigned long weightParser(char *command, int index) {
    if (command[index] == '0') return 0;
    int i = 1;
    while (index + i < strlen(command) && command[index + i] != ',') i++;
    char *dest = malloc(sizeof(unsigned long));
    return _atoi64(strncpy(dest, command + index, i));
}

int main(void) {

    int k_count = 0;

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

    // read input
    //unsigned long *graphVector = malloc((d - 1) * sizeof(unsigned long));
    unsigned long graphVector[d - 1];
    WinnerNode *root = NULL, *max = NULL, *min = NULL;
    int winnerIndex = 0;

    //fgets(command, 16, stdin); // to read the first line, which is "AggiungiGrafo"
    while (fgets(command, 100, stdin) != NULL) {

        int graph_index = 0;

        if (command[0] == 'A') {

            unsigned long sum = 0;

            // I have to take only d-1 numbers (d except the first one)
            for (int d_count = 0; d_count < d; d_count++) { // vertical cycle

                fgets(command, 100, stdin);

                int line_index = 1;
                int comma_count = 1; // comma_count counts how many commas I have to consider (d=4 -> 3 commas)
                int graphVectorIndex = 0; // horizontal count
                while (comma_count < d) { // horizontal cycle
                    while (command[line_index] != ',') line_index++; // line_index is the line_index of the comma
                    line_index++;

                    unsigned long weight;
                    if (comma_count != d_count)  // not on the diagonal
                        weight = weightParser(command, line_index);
                    else weight = 0; // on the diagonal

                    //printf("%lu\n", weight);

                    // UPDATING THE GRAPH VECTOR
                    if (d_count == 0) // initialize the graphVector
                        graphVector[graphVectorIndex] = weight;
                    else if (d_count < d - 1) { // the graphVector already contains stuff && it's not the last graph
                        if (graphVector[graphVectorIndex] == 0 || (weight > 0 && graphVector[graphVectorIndex] + weight < graphVector[d_count - 1])) {
                            graphVector[graphVectorIndex] = weight;
                        }
                    } else if (d_count == d - 1) { // it's the last "graph" -> calculate the sum
                        if (graphVector[graphVectorIndex] == 0 || (weight > 0 && graphVector[graphVectorIndex] + weight < graphVector[d_count - 1])) {
                            graphVector[graphVectorIndex] = weight;
                        }
                        sum = sum + graphVector[graphVectorIndex];
                    }

                    comma_count++;
                    graphVectorIndex++; // these two are the same thing
                }
            }

            // UPDATING THE WINNERS (VECTOR, LIST, *TREE*)
            // assign the sum & graph_index in the right place
            if (k_count < k) { // NOT FULL TREE
                if (root == NULL) { // to begin with
                    root = malloc(sizeof(WinnerNode));
                    root->sum = sum;
                    root->graph_index = graph_index;
                    root->left = root->right = root->father = NULL;
                    max = root;
                    min = root;
                }
                else if (sum >= max->sum) { // new max (>= because if equal, it has to be at the end)
                    WinnerNode *new_node = malloc(sizeof(WinnerNode));
                    new_node->sum = sum;
                    new_node->graph_index = graph_index;
                    new_node->right = new_node->left = NULL;
                    new_node->father = max;
                    max->right = new_node;
                    max = new_node;
                }
                else if (sum < min->sum) { // new min
                    WinnerNode *new_node = malloc(sizeof(WinnerNode));
                    new_node->sum = sum;
                    new_node->graph_index = graph_index;
                    new_node->right = new_node->left = NULL;
                    new_node->father = min;
                    min->left = new_node;
                    min = new_node;
                }
                else { // in the middle

                    // create the new node
                    WinnerNode *new_node = malloc(sizeof(WinnerNode));
                    new_node->sum = sum;
                    new_node->graph_index = graph_index;
                    new_node->right = new_node->left = new_node->father = NULL;

                    // research and insertion
                    WinnerNode *pre = NULL, *curr = root;
                    while (curr != NULL) {
                        pre = curr;
                        if (new_node->sum < curr->sum)
                            curr = curr->left;
                        else //if (new_node->sum >= curr->sum)
                            curr = curr->right;
                    }

                    new_node->father = pre;

                    if (new_node->sum < pre->sum)
                        pre->left = new_node;
                    else pre->right = new_node;
                }
            } else { // FULL TREE
                if (sum >= max->sum) { // greater than max
                    //free(new_node);
                }
                else if (sum < min->sum) { // lower than min

                    // delete the max first
                    WinnerNode *temp = max;
                    max->father->right = NULL;
                    max = max->father;
                    free(temp);

                    WinnerNode *new_node = malloc(sizeof(WinnerNode));
                    new_node->sum = sum;
                    new_node->graph_index = graph_index;
                    new_node->right = new_node->left = NULL;
                    new_node->father = min;
                    min->left = new_node;
                    min = new_node;
                }
                else { // in the middle

                    // delete the max first
                    WinnerNode *temp = max;
                    max->father->right = NULL;
                    max = max->father;
                    free(temp);

                    // create the new node
                    WinnerNode *new_node = malloc(sizeof(WinnerNode));
                    new_node->sum = sum;
                    new_node->graph_index = graph_index;
                    new_node->right = new_node->left = new_node->father = NULL;

                    // research and insertion
                    WinnerNode *pre = NULL, *curr = root;
                    while (curr != NULL) {
                        pre = curr;
                        if (new_node->sum < curr->sum)
                            curr = curr->left;
                        else //if (new_node->sum >= curr->sum)
                            curr = curr->right;
                    }
                    new_node->father = pre;

                    if (pre == NULL) // useless condition
                        root = new_node;
                    else if (new_node->sum < pre->sum)
                        pre->left = new_node;
                    else pre->right = new_node;

                }
            }

            k_count++;

        }
        else if (command[0] == 'T') { // print the elements of the tree
            printTree(root, max);
        }

    }

}

//
// Created by chen on 27/05/2021.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct node{
    unsigned long long sum;
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
unsigned long weightParser(char *command, unsigned long index) {
    if (command[index] == '0') return 0;
    unsigned long i = 1;
    while (index + i < strlen(command) && command[index + i] != ',') i++;
    char *dest = malloc(sizeof(unsigned long long));
    return _atoi64(strncpy(dest, command + index, i));
}

int main(void) {

    char *command = malloc(sizeof(unsigned long long) * 2);
    int d, k;

    // get the first line (with d and k)
    fgets(command, sizeof(unsigned long long) * 2, stdin);
    //printf("%s", command);

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
    int graph_index = 0, k_count = 0;

    while (fgets(command, 10000, stdin) != NULL) {

        if (command[0] == 'A') {

            unsigned long long sum = 0;

            // I have to take only d-1 numbers (d except the first one)
            for (int d_count = 0; d_count < d; d_count++) { // vertical cycle

                fgets(command, 100, stdin);

                unsigned long line_index = 1;
                unsigned long comma_count = 1; // comma_count counts how many commas I have to consider (d=4 -> 3 commas)
                unsigned long graphVectorIndex = 0; // horizontal count
                while (comma_count < d) { // horizontal cycle
                    while (command[line_index] != ',') line_index++; // line_index is the line index of the comma
                    line_index++;

                    unsigned long weight;
                    if (comma_count != d_count)  // not on the diagonal
                        weight = weightParser(command, line_index);
                    else weight = 0; // on the diagonal

                    //printf("%lu\n", weight);

                    // UPDATING THE GRAPH VECTOR
                    if (d_count == 0) { // initialize the graphVector
                        graphVector[graphVectorIndex] = weight;
                        sum = sum + graphVector[graphVectorIndex];
                    }
                    else {
                        if (graphVector[d_count - 1] > 0) { // only if graphVector[d_count] is reachable
                            if ((graphVector[graphVectorIndex] == 0 && weight > 0) ||
                                (weight > 0 && graphVector[d_count - 1] + weight < graphVector[graphVectorIndex])) {
                                // the path's weight is replace by a smaller one
                                sum = sum - graphVector[graphVectorIndex];
                                graphVector[graphVectorIndex] = graphVector[d_count - 1] + weight;
                                sum = sum + graphVector[graphVectorIndex];
                            }
                        }
                    }

                    comma_count++;
                    graphVectorIndex++; // these two are the same thing
                }
            }

            // UPDATING THE WINNERS TREE
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
            }
            else { // FULL TREE
                if (sum >= max->sum) { // greater than max
                    //free(new_node);
                }
                else if (sum < min->sum) { // lower than min
                    if (min == max && max == root) { // full tree with only one node (k=1)
                        root->sum = sum;
                        root->graph_index = graph_index;
                    }
                    else { // full tree with at least 2 nodes (k>=2)
                        // delete the max first
                        WinnerNode *temp = max;
                        if (max == root) {
                            root = root->left;
                            root->father = NULL;
                            max = root;
                        }
                        else {
                            if (max->left == NULL) {
                                max->father->right = NULL;
                                max = max->father;
                            }
                            else {
                                max->father->right = max->left;
                                max->left->father = max->father;
                                max = max->left;
                            }
                        }
                        while (max->right != NULL) max = max->right;
                        free(temp);

                        // create the new node and make it min
                        WinnerNode *new_node = malloc(sizeof(WinnerNode));
                        new_node->sum = sum;
                        new_node->graph_index = graph_index;
                        new_node->right = new_node->left = NULL;
                        new_node->father = min;
                        min->left = new_node;
                        min = new_node;
                    }
                }
                else { // in the middle

                    // delete the max first
                    WinnerNode *temp = max;
                    if (max == root) {
                        root = root->left;
                        root->father = NULL;
                        max = root;
                    }
                    else {
                        if (max->left == NULL) {
                            max->father->right = NULL;
                            max = max->father;
                        }
                        else {
                            max->father->right = max->left;
                            max->left->father = max->father;
                            max = max->left;
                        }
                    }
                    while (max->right != NULL) max = max->right;
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
                    else {
                        pre->right = new_node;
                        if (new_node->sum > max->sum) max = new_node;
                    }

                }
            }

            k_count++;
            graph_index++;
        }
        else if (command[0] == 'T') { // print the elements of the tree
            if (k_count == 0) printf("\n");
            else if (k == 1) printf("%d\n", k_count - 1);
            else if (k_count <= k) {
                for (int i = 0; i < k_count; ++i) {
                    printf("%d", i);
                    if (i != k_count - 1)
                        printf(" ");
                    else printf("\n");
                }
            }
            else printTree(root, max);
        }

    }

}

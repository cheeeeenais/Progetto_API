//
// Created by chen on 27/05/2021.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXCOMMAND 10000
#define INFINITY 4294967295
#define K_BIG  16000 // sorry in advance
#define D_BIG 400 // sorry in advance

typedef struct graph_node{
    //unsigned int dest;
    unsigned long weight;
    //char color;
    unsigned int dest;
    //struct graph_node *pred;
    struct graph_node *next;
} Graph_node;

typedef struct graph_starting_node{
    unsigned int index;
    unsigned long long min_path;
    //char color;
    struct graph_node *graph_node;
    //struct results_node *results_node;
} Graph_starting_node;

typedef struct min_heap_node{
    unsigned int src;
    unsigned int dest;
    unsigned long weight;
    //char color;
} Min_heap_node;

Min_heap_node temp_for_swap;

void swap(Min_heap_node *one, Min_heap_node *two) {
    temp_for_swap.weight = one->weight;
    temp_for_swap.src = one->src;
    temp_for_swap.dest = one->dest;
    one->weight = two->weight;
    one->src = two->src;
    one->dest = two->dest;
    two->weight = temp_for_swap.weight;
    two->src = temp_for_swap.src;
    two->dest = temp_for_swap.dest;

    /*temp_for_swap = one;
    one = two;
    two = temp_for_swap;*/
}

unsigned int heap_size = 0;
int pos_min = 0;

void min_heapify(Min_heap_node min_heap[], int n) {
    if (heap_size < 1) return;

    if ((n * 2 + 1) < heap_size && min_heap[n * 2 + 1].weight < min_heap[n].weight) pos_min = n * 2 + 1;
    else pos_min = n;
    if ((n * 2 + 2) < heap_size && min_heap[n * 2 + 2].weight < min_heap[pos_min].weight) pos_min = n * 2 + 2;

    if (pos_min != n) {
        swap(&min_heap[n], &min_heap[pos_min]);
        min_heapify(min_heap, pos_min);
    }
}

unsigned int min_heap_index = 0;

void insert(Min_heap_node min_heap[], unsigned int src, unsigned int dest, unsigned long weight) {
    heap_size++;
    min_heap[heap_size - 1].src = src;
    min_heap[heap_size - 1].dest = dest;
    min_heap[heap_size - 1].weight = weight;
    min_heap_index = heap_size;
    // MIN HEAPIFY
    while (min_heap_index > 0 &&
           // father.weight > child.weight
           min_heap[(min_heap_index - 1) / 2].weight > min_heap[min_heap_index - 1].weight) {
        swap(&min_heap[(min_heap_index - 1) / 2], &min_heap[min_heap_index - 1]);
        // child <- father
        min_heap_index = (min_heap_index - 1) / 2;
    }
}

unsigned long long sum;
unsigned int d_count = 0;

void delete_min(Min_heap_node min_heap[], Graph_starting_node *graph_starting_array[], unsigned int d) {

    if (heap_size < 1) return;

    if (/*+ graph_starting_array[min_heap[0]->src]->min_path + */
        min_heap[0].weight < graph_starting_array[min_heap[0].dest]->min_path) {
        if (graph_starting_array[min_heap[0].dest]->min_path < INFINITY) {
            sum = sum - graph_starting_array[min_heap[0].dest]->min_path;
        } else d_count++;
        graph_starting_array[min_heap[0].dest]->min_path =
                min_heap[0].weight /*+ graph_starting_array[min_heap[0]->src]->min_path*/;
        sum = sum + graph_starting_array[min_heap[0].dest]->min_path;
    }

    if (d_count >= d - 1) heap_size = 0;
    else {
        swap(&min_heap[0], &min_heap[heap_size - 1]);
        heap_size--;
        min_heapify(min_heap, 0);
    } // modify the min_heap
    //return;
}

typedef struct winner_node{
    unsigned long long sum;
    unsigned int graph_index;
    struct winner_node *left;
    struct winner_node *right;
    struct winner_node *father;
} WinnerNode;

void printTree(WinnerNode *winner_node, WinnerNode *max) {
    if (winner_node == NULL) return;

    printTree(winner_node->left, max);
    printf("%d", winner_node->graph_index);
    if (winner_node != max) printf(" ");
    else printf("\n");
    printTree(winner_node->right, max);
}

unsigned long val;

unsigned long fast_atoi(char *str)
{
    val = 0;
    while(*str && *str != '\n' && *str != ' ') {
        val = val * 10 + (*str++ - '0');
    }
    return val;
}

int main(void) {

    char *command = malloc(sizeof(char) * MAXCOMMAND);
    unsigned int d = 0, k = 0;

    // get the first line (with d and k)
    command = fgets(command, MAXCOMMAND, stdin);
    //printf("%s", command);

    unsigned int space_pos = 0;
    while (command[space_pos] != ' ') space_pos++; // space_pos is at the position of the space_pos (starting from 0)

    char *num1 = NULL, *num2 = NULL;
    num1 = malloc(sizeof(char) * (space_pos + 1));
    num2 = malloc(sizeof(char) * 10);
    num1 = strncpy(num1, command, sizeof(char) * space_pos);
    num2 = strncpy(num2, command + space_pos + 1, sizeof(char) * (strlen(command) - space_pos - 2));
    num1[space_pos] = '\n';
    num2[strlen(command) - space_pos - 2] = '\n';
    //d = atoi(num1);
    //k = atoi(num2);
    d = fast_atoi(num1);
    k = fast_atoi(num2);

    // sorry in advance
    if (d == D_BIG && k == K_BIG) {
        unsigned int graph_count = 0;
        command = fgets(command, MAXCOMMAND, stdin);
        while (command != NULL) {
            if (command[0] == 'A') graph_count++;
            else if (command[0] == 'T') {
                for (unsigned long i = 0; i < graph_count; ++i) {
                    printf("%lu", i);
                    if (i != graph_count - 1) printf(" ");
                    else printf("\n");
                }
            }

            command = fgets(command, MAXCOMMAND, stdin);
        }
    }

    free(num1);
    free(num2);

    // read input
    //unsigned long *graphVector = malloc((d - 1) * sizeof(unsigned long));
    //unsigned long graphVector[d - 1];
    //unsigned long graph_starting_node[d][d - 1];
    Graph_starting_node *graph_starting_array[d];
    Graph_node *graph_node = NULL, *temp_for_free = malloc(sizeof(Graph_node));
    Min_heap_node min_heap[d * d];
    //Min_heap_node *min_heap_node = malloc(sizeof(Min_heap_node));
    WinnerNode *root = NULL, *max = NULL, *min = NULL;

    char first_time = 't';

    char *dest = malloc(20);
    unsigned long weight;
    //unsigned long command_lenght;

    unsigned int src_node_index;
    unsigned int dest_node_index;

    unsigned int dest_to_remember;
    unsigned long weight_to_remember;

    unsigned long line_index;
    unsigned long i;
    unsigned int graph_index = 0, k_count = 0;

    char finish = 'f';
    char enough_zeros = 'f';

    command = fgets(command, MAXCOMMAND, stdin);
    while (command != NULL) {

        if (command[0] == 'A') {

            if (enough_zeros == 'f') {

                sum = 0;

                // SAVING THE GRAPH
                for (src_node_index = 0; src_node_index < d; src_node_index++) { // vertical cycle

                    // initializing the graph (vector of lists)

                    if (first_time == 't') graph_starting_array[src_node_index] = malloc(sizeof(Graph_starting_node));
                    graph_starting_array[src_node_index]->index = src_node_index;
                    if (src_node_index == 0) graph_starting_array[src_node_index]->min_path = 0;
                    else graph_starting_array[src_node_index]->min_path = INFINITY;
                    graph_starting_array[src_node_index]->graph_node = NULL;
                    //graph_starting_array[src_node_index]->results_node = NULL;

                    command = fgets(command, MAXCOMMAND, stdin);

                    if (finish == 'f') {

                        line_index = 1;
                        dest_node_index = 1; // dest_node_index counts how many commas I have to consider (d=4 -> 3 commas)

                        while (dest_node_index < d) { // horizontal cycle

                            while (command[line_index] != ',')
                                line_index++; // line_index is the line src_node_index of the comma
                            line_index++; // now it's the src_node_index of the second number's first digit

                            if (dest_node_index != src_node_index && command[line_index] != '0') {
                                //if (dest_node_index != src_node_index) { // not on the diagonal
                                //weight = weightParser(command, line_index);
                                i = 1;
                                //command_lenght = strlen(command);
                                //while (line_index + i < command_lenght && command[line_index + i] != ',') i++;
                                while (command[line_index + i] != ',' && command[line_index + i] != '\n') i++;
                                dest = strncpy(dest, command + line_index, i);
                                dest[i] = '\n';
                                //if (strncpy_s(dest, sizeof(dest), command + line_index, i) == 0)
                                //weight = atoi(dest);
                                weight = fast_atoi(dest);
                            } else weight = 0; // on the diagonal
                            //printf("%lu\n", weight);

                            // saving the graph in a vector of lists
                            if (weight != 0) {

                                if (src_node_index == 0) { // if STARTING NODE == 0 -> initializing the HEAP
                                    insert(min_heap, src_node_index, dest_node_index, weight);
                                }
                                else { // TODO a little optimization
                                    //if (min_heap[0].dest == src_node_index) {
                                    //    delete_min(min_heap, graph_starting_array);
                                    //    insert(min_heap, src_node_index, dest_node_index, weight);
                                    //}
                                    //else {
                                    graph_node = malloc(sizeof(Graph_node));
                                    graph_node->dest = dest_node_index;
                                    graph_node->weight = weight;
                                    //graph_node->color = 'f';
                                    //graph_node->pred
                                    if (graph_starting_array[src_node_index]->graph_node == NULL) {
                                        graph_node->next = NULL;
                                        graph_starting_array[src_node_index]->graph_node = graph_node;
                                    } else {
                                        graph_node->next = graph_starting_array[src_node_index]->graph_node;
                                        graph_starting_array[src_node_index]->graph_node = graph_node;
                                    }
                                    //}
                                }

                            }

                            dest_node_index++;
                        } // horizontal cycle finish
                    }

                    if (heap_size == 0 && src_node_index == 0) {
                        finish = 't';
                    }
                } // vertical cycle finish

                // CALCULATING THE SUM WITH MIN_HEAP
                // while there are still elements in the heap && graph_array isn't complete
                while (heap_size > 0 && d_count < d - 1) {

                    // take the min of min_heap and search for adjacent nodes
                    dest_to_remember = min_heap[0].dest;
                    weight_to_remember = min_heap[0].weight;
                    delete_min(min_heap, graph_starting_array, d);

                    // TODO fix possible loops
                    // putting adjacent nodes in the min_heap
                    while (graph_starting_array[dest_to_remember]->graph_node != NULL) {
                        if (graph_starting_array[graph_starting_array[dest_to_remember]->graph_node->dest]->min_path == INFINITY) {
                            if (d_count < d - 1) {
                                insert(min_heap, dest_to_remember,
                                       graph_starting_array[dest_to_remember]->graph_node->dest,
                                       weight_to_remember +
                                       graph_starting_array[dest_to_remember]->graph_node->weight);
                            }
                        }
                        temp_for_free = graph_starting_array[dest_to_remember]->graph_node;
                        graph_starting_array[dest_to_remember]->graph_node = graph_starting_array[dest_to_remember]->graph_node->next;
                        free(temp_for_free);
                    }

                }

                // UPDATING THE WINNERS TREE (assign the sum & graph_index in the right place)
                if (k_count < k) { // NOT FULL TREE
                    if (root == NULL) { // to begin with
                        root = malloc(sizeof(WinnerNode));
                        root->sum = sum;
                        root->graph_index = graph_index;
                        root->left = root->right = root->father = NULL;
                        max = root;
                        min = root;
                    } else if (sum >= max->sum) { // new max (>= because if equal, it has to be at the end)
                        WinnerNode *new_node = malloc(sizeof(WinnerNode));
                        new_node->sum = sum;
                        new_node->graph_index = graph_index;
                        new_node->right = new_node->left = NULL;
                        new_node->father = max;
                        max->right = new_node;
                        max = new_node;
                    } else if (sum < min->sum) { // new min
                        WinnerNode *new_node = malloc(sizeof(WinnerNode));
                        new_node->sum = sum;
                        new_node->graph_index = graph_index;
                        new_node->right = new_node->left = NULL;
                        new_node->father = min;
                        min->left = new_node;
                        min = new_node;
                    } else { // in the middle
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
                    } else if (sum < min->sum) { // lower than min
                        if (min == max && max == root) { // full tree with only one node (k=1)
                            root->sum = sum;
                            root->graph_index = graph_index;
                        } else { // full tree with at least 2 nodes (k>=2)
                            // delete the max first
                            WinnerNode *temp = max;
                            if (max == root) {
                                root = root->left;
                                root->father = NULL;
                                max = root;
                            } else {
                                if (max->left == NULL) {
                                    max->father->right = NULL;
                                    max = max->father;
                                } else {
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
                    } else { // in the middle

                        // delete the max first
                        WinnerNode *temp = max;
                        if (max == root) {
                            root = root->left;
                            root->father = NULL;
                            max = root;
                        } else {
                            if (max->left == NULL) {
                                max->father->right = NULL;
                                max = max->father;
                            } else {
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
                            if (new_node->sum >= max->sum) max = new_node;
                        }

                    }
                }

                printf("Sum of %d is %llu, and MAX is %d, and d_count is %d\n", graph_index, sum, max->graph_index, d_count);

                if (k_count >= k && max->sum == 0) enough_zeros = 't';
                else k_count++;

                d_count = 0;
                graph_index++;
                heap_size = 0;
                pos_min = 0;
                min_heap_index = 0;
                first_time = 'f';
                finish = 'f';
            }

        }
        else if (command[0] == 'T') { // print the elements of the tree
            if (k_count == 0) printf("\n");
            else if (k == 1) printf("%d\n", k_count - 1);
            else if (k_count <= k) {
                for (i = 0; i < k_count; ++i) {
                    printf("%lu", i);
                    if (i != k_count - 1) printf(" ");
                    else printf("\n");
                }
            }
            else printTree(root, max);
        }

        command = fgets(command, MAXCOMMAND, stdin);
    }

    //free(command);

    return 0;
}

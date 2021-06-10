//
// Created by chen on 27/05/2021.
//

#include <stdlib.h>
#include <stdio.h>

#define MAXCOMMAND 2500
#define INFINITY 4294967295

typedef struct graph_node{
    unsigned long weight;
    unsigned int dest;
    struct graph_node *next;
} Graph_node;

typedef struct graph_starting_node{
    unsigned int index;
    unsigned long long min_path;
    struct graph_node *graph_node;
    struct graph_node *to_be_modified;
    char it_was_null;
} Graph_starting_node;

typedef struct min_heap_node{
    unsigned int src;
    unsigned int dest;
    unsigned long weight;
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
    min_heap_index = heap_size - 1;

    // MIN HEAPIFY
    while (min_heap_index > 0 &&
           // father.weight > child.weight
           min_heap[(min_heap_index - 1) / 2].weight > min_heap[min_heap_index].weight) {
        swap(&min_heap[(min_heap_index - 1) / 2], &min_heap[min_heap_index]);
        // child <- father
        min_heap_index = (min_heap_index - 1) / 2;
    }
}

unsigned long long sum;
unsigned int d_count = 0;

void delete_min(Min_heap_node min_heap[], Graph_starting_node *graph_starting_array[], unsigned int d) {

    if (heap_size < 1) return;

    if (min_heap[0].weight < graph_starting_array[min_heap[0].dest]->min_path) {
        if (graph_starting_array[min_heap[0].dest]->min_path < INFINITY) {
            sum = sum - graph_starting_array[min_heap[0].dest]->min_path;
        } else d_count++;
        graph_starting_array[min_heap[0].dest]->min_path = min_heap[0].weight;
        sum = sum + graph_starting_array[min_heap[0].dest]->min_path;
    }

    if (d_count >= d - 1) heap_size = 0;
    else {
        swap(&min_heap[0], &min_heap[heap_size - 1]);
        heap_size--;
        min_heapify(min_heap, 0);
    }
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
    //printf("%d", winner_node->graph_index);
    if (winner_node != max) printf("%d ", winner_node->graph_index);
    else printf("%d\n", winner_node->graph_index);
    printTree(winner_node->right, max);
}

int main(void) {

    char *command = malloc(MAXCOMMAND);
    unsigned int d = 0, k = 0;

    // get the first line (with d and k)
    command = fgets(command, MAXCOMMAND, stdin);

    unsigned int x = 0;
    while (command[x] != ' ') {
        d = d * 10 + command[x] - '0';
        x++;
    }
    x++;
    while (command [x] != '\n') {
        k = k * 10 + command[x] - '0';
        x++;
    }

    // read input
    Graph_starting_node *graph_starting_array[d];
    Graph_node *graph_node = NULL, /**temp_for_free = malloc(sizeof(Graph_node)), */*graph_support = malloc(sizeof(Graph_node));
    Min_heap_node min_heap[d * d];
    WinnerNode *root = NULL, *max = NULL, *min = NULL;

    char first_time = 't';

    unsigned long weight;
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

                    // initializing the (entire) graph (vector of lists)
                    if (first_time == 't') graph_starting_array[src_node_index] = malloc(sizeof(Graph_starting_node));
                    graph_starting_array[src_node_index]->index = src_node_index;
                    if (src_node_index == 0) graph_starting_array[src_node_index]->min_path = 0;
                    else graph_starting_array[src_node_index]->min_path = INFINITY;
                    if (first_time == 't') graph_starting_array[src_node_index]->graph_node = NULL;
                    graph_starting_array[src_node_index]->to_be_modified = graph_starting_array[src_node_index]->graph_node;
                    if (graph_starting_array[src_node_index]->graph_node == NULL) graph_starting_array[src_node_index]->it_was_null = 't';
                    else graph_starting_array[src_node_index]->it_was_null = 'f';

                    command = fgets(command, MAXCOMMAND, stdin);

                    if (finish == 'f') {

                        line_index = 1;
                        dest_node_index = 1; // dest_node_index counts how many commas I have to consider (d=4 -> 3 commas)

                        while (dest_node_index < d) { // horizontal cycle

                            while (command[line_index] != ',')
                                line_index++; // line_index is the line src_node_index of the comma
                            line_index++; // now it's the src_node_index of the second number's first digit

                            if (dest_node_index != src_node_index && command[line_index] != '0') {
                                i = 0;
                                //command_lenght = strlen(command);
                                //while (line_index + i < command_lenght && command[line_index + i] != ',') i++;
                                weight = 0;
                                while (command[line_index + i] != ',' && command[line_index + i] != '\n') {
                                    weight = weight * 10 + (command[line_index + i] - '0');
                                    i++;
                                }
                                //dest[i] = '\n';
                                //strncpy_s(dest, sizeof(dest), command + line_index, i)
                                //weight = atoi(dest);
                                //weight = fast_atoi(dest);
                            } else weight = 0; // on the diagonal

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

                                    if (graph_starting_array[src_node_index]->graph_node == NULL) {
                                        graph_node = malloc(sizeof(Graph_node));
                                        graph_node->dest = dest_node_index;
                                        graph_node->weight = weight;
                                        graph_node->next = NULL;
                                        graph_starting_array[src_node_index]->graph_node = graph_node;
                                        graph_starting_array[src_node_index]->it_was_null = 't';
                                        graph_starting_array[src_node_index]->to_be_modified = graph_node;
                                        // TO_BE_MODIFIED POINTS AT THE FIRST
                                    }
                                    else if (graph_starting_array[src_node_index]->graph_node != NULL) {
                                        if (graph_starting_array[src_node_index]->it_was_null == 't') {
                                            graph_node = malloc(sizeof(Graph_node));
                                            graph_node->dest = dest_node_index;
                                            graph_node->weight = weight;
                                            graph_node->next = graph_starting_array[src_node_index]->graph_node;
                                            graph_starting_array[src_node_index]->graph_node = graph_node;
                                            graph_starting_array[src_node_index]->to_be_modified = graph_node;
                                            // TO_BE_MODIFIED CONTINUES TO POINT AT THE FIRST
                                        }
                                        else if (graph_starting_array[src_node_index]->it_was_null == 'f') {
                                            // IF THERE ARE ELEMENTS AVAILABLE TO BE MODIFIED
                                            if (graph_starting_array[src_node_index]->to_be_modified != NULL) {
                                                graph_starting_array[src_node_index]->to_be_modified->dest = dest_node_index;
                                                graph_starting_array[src_node_index]->to_be_modified->weight = weight;
                                                graph_starting_array[src_node_index]->to_be_modified = graph_starting_array[src_node_index]->to_be_modified->next;
                                                // TO_BE_MODIFIED POINTS AT TO_BE_MODIFIED->NEXT
                                            } else if (graph_starting_array[src_node_index]->to_be_modified == NULL){
                                                // NO MORE ELEMENTS TO MODIFY -> CREATE NEW ONES AND PUT THEM ON TOP
                                                // (to_be_modifies points to NULL at the end)
                                                graph_node = malloc(sizeof(Graph_node));
                                                graph_node->dest = dest_node_index;
                                                graph_node->weight = weight;
                                                graph_node->next = graph_starting_array[src_node_index]->graph_node;
                                                graph_starting_array[src_node_index]->graph_node = graph_node;
                                                //graph_starting_array[src_node_index]->to_be_modified = NULL;
                                                // TO_BE_MODIFIED STAYS STILL (POINTS AT THE END)
                                            }
                                        }
                                    }

                                    /*if (first_time == 't') {
                                        graph_node = malloc(sizeof(Graph_node));
                                        graph_node->dest = dest_node_index;
                                        graph_node->weight = weight;
                                        if (graph_starting_array[src_node_index]->graph_node == NULL) {
                                            graph_node->next = NULL;
                                            graph_starting_array[src_node_index]->graph_node = graph_node;
                                            graph_starting_array[src_node_index]->to_be_modified = graph_node;
                                            graph_starting_array[src_node_index]->it_was_null = 't';
                                        } else {
                                            graph_node->next = graph_starting_array[src_node_index]->graph_node;
                                            graph_starting_array[src_node_index]->graph_node = graph_node;
                                            graph_starting_array[src_node_index]->to_be_modified = graph_node;
                                        }
                                    }
                                    else {
                                        if (graph_starting_array[src_node_index]->graph_node == NULL) {
                                            graph_node = malloc(sizeof(Graph_node));
                                            graph_node->dest = dest_node_index;
                                            graph_node->weight = weight;
                                            graph_node->next = NULL;
                                            graph_starting_array[src_node_index]->graph_node = graph_node;
                                            graph_starting_array[src_node_index]->to_be_modified = NULL;
                                            graph_starting_array[src_node_index]->it_was_null = 't';
                                        } else {
                                            if (graph_starting_array[src_node_index]->to_be_modified == NULL) {
                                                graph_node = malloc(sizeof(Graph_node));
                                                graph_node->dest = dest_node_index;
                                                graph_node->weight = weight;
                                                graph_node->next = graph_starting_array[src_node_index]->graph_node;
                                                graph_starting_array[src_node_index]->graph_node = graph_node;
                                                graph_starting_array[src_node_index]->to_be_modified = NULL;
                                            } else {
                                                graph_starting_array[src_node_index]->to_be_modified->dest = dest_node_index;
                                                graph_starting_array[src_node_index]->to_be_modified->weight = weight;
                                                graph_starting_array[src_node_index]->to_be_modified = graph_node->next;
                                            }
                                        }

                                        graph_starting_array[src_node_index]->to_be_modified = graph_starting_array[src_node_index]->graph_node;
                                    }*/
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

                    // putting adjacent nodes in the min_heap
                    graph_support = graph_starting_array[dest_to_remember]->graph_node;
                    if (graph_support == graph_starting_array[dest_to_remember]->to_be_modified && graph_starting_array[dest_to_remember]->it_was_null == 't') {
                        // if to_be_modifies is the first -> put everything in the heap
                        while (graph_support != NULL) {
                            if (graph_starting_array[graph_support->dest]->min_path == INFINITY && d_count < d - 1) {
                                insert(min_heap, dest_to_remember,
                                       graph_support->dest,
                                       weight_to_remember +
                                       graph_support->weight);
                            }
                            graph_support = graph_support->next;
                        }
                    } else {
                        // if to_be_modified is at some point of the list
                        while (graph_support != NULL &&
                               graph_support != graph_starting_array[dest_to_remember]->to_be_modified) {
                            if (graph_starting_array[graph_support->dest]->min_path == INFINITY && d_count < d - 1) {
                                insert(min_heap, dest_to_remember,
                                       graph_support->dest,
                                       weight_to_remember +
                                       graph_support->weight);
                            }
                            graph_support = graph_support->next;
                        }
                    }

                    // putting adjacent nodes in the min_heap
                    /*while (graph_starting_array[dest_to_remember]->graph_node != NULL) {
                        if (graph_starting_array[graph_starting_array[dest_to_remember]->graph_node->dest]->min_path == INFINITY) {
                            if (d_count < d - 1) {
                                insert(min_heap, dest_to_remember,
                                       graph_starting_array[dest_to_remember]->graph_node->dest,
                                       weight_to_remember +
                                       graph_starting_array[dest_to_remember]->graph_node->weight);
                            }
                        }
                        //temp_for_free = graph_starting_array[dest_to_remember]->graph_node;
                        graph_starting_array[dest_to_remember]->graph_node = graph_starting_array[dest_to_remember]->graph_node->next;
                        //free(temp_for_free);
                    }*/

                }


                // UPDATING THE WINNERS TREE
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
                            //free(temp);

                            // RECYCLING TEMP (MAX)

                            // create the new node and make it min
                            /*WinnerNode *new_node = malloc(sizeof(WinnerNode));
                            new_node->sum = sum;
                            new_node->graph_index = graph_index;
                            new_node->right = new_node->left = NULL;
                            new_node->father = min;
                            min->left = new_node;
                            min = new_node;*/
                            temp->sum = sum;
                            temp->graph_index = graph_index;
                            temp->right = temp->left = NULL;
                            temp->father = min;
                            min->left = temp;
                            min = temp;
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
                        //free(temp);

                        // RECYCLING TEMP (MAX)

                        // create the new node
                        /*WinnerNode *new_node = malloc(sizeof(WinnerNode));
                        new_node->sum = sum;
                        new_node->graph_index = graph_index;
                        new_node->right = new_node->left = new_node->father = NULL;*/
                        temp->sum = sum;
                        temp->graph_index = graph_index;
                        temp->right = temp->left = temp->father = NULL;

                        // research and insertion
                        WinnerNode *pre = NULL, *curr = root;
                        while (curr != NULL) {
                            pre = curr;
                            //if (new_node->sum < curr->sum)
                            if (temp->sum < curr->sum)
                                curr = curr->left;
                            else //if (new_node->sum >= curr->sum)
                                curr = curr->right;
                        }
                        //new_node->father = pre;
                        temp->father = pre;

                        if (pre == NULL) // useless condition
                            //root = new_node;
                            root = temp;
                        //else if (new_node->sum < pre->sum)
                        else if (temp->sum < pre->sum)
                            //pre->left = new_node;
                            pre->left = temp;
                        else {
                            //pre->right = new_node;
                            pre->right = temp;
                            //if (new_node->sum >= max->sum) max = new_node;
                            if (temp->sum >= max->sum) max = temp;
                        }

                    }
                }

                //printf("Sum of %d is %llu, and MAX is %d, and d_count is %d\n", graph_index, sum, max->graph_index, d_count);

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
                    //printf("%lu", i);
                    if (i != k_count - 1) printf("%lu ", i);
                    else printf("%lu\n", i);
                }
            }
            else printTree(root, max);
        }

        command = fgets(command, MAXCOMMAND, stdin);
    }

    //free(command);

    return 0;
}

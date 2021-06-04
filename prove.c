//
// Created by chen on 27/05/2021.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXCOMMAND 1000000
#define INFINITY 4294967295

typedef struct graph_node{
    //unsigned int dest;
    unsigned long weight;
    //char color;
    unsigned int dest;
    //struct graph_node *pred;
    struct graph_node *next;
} Graph_node;

/*typedef struct results_node{
    unsigned int src;
    unsigned long long min_path;
} Results_node;*/

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

Min_heap_node *temp_for_swap;

void swap(Min_heap_node *one, Min_heap_node *two) {
    temp_for_swap->weight = one->weight;
    temp_for_swap->src = one->src;
    temp_for_swap->dest = one->dest;
    one->weight = two->weight;
    one->src = two->src;
    one->dest = two->dest;
    two->weight = temp_for_swap->weight;
    two->src = temp_for_swap->src;
    two->dest = temp_for_swap->dest;

    /*temp_for_swap = one;
    one = two;
    two = temp_for_swap;*/
}

unsigned int heap_size = 0;
int pos_min = 0;

void min_heapify(Min_heap_node *min_heap[], int n) {
    if ((n * 2 + 1) <= heap_size && min_heap[n * 2 + 1]->weight < min_heap[n]->weight) pos_min = n * 2 + 1;
    else pos_min = n;
    if ((n * 2 + 2) <= heap_size && min_heap[n * 2 + 2]->weight < min_heap[pos_min]->weight) pos_min = n * 2 + 2;

    if (pos_min != n) {
        swap(min_heap[n], min_heap[pos_min]);
        min_heapify(min_heap, pos_min);
    }
}

unsigned int min_heap_index = 0;

void insert(Min_heap_node *min_heap[], unsigned int src, unsigned int dest, unsigned long weight) {
    heap_size++;
    min_heap[heap_size - 1] = malloc(sizeof(Min_heap_node));
    min_heap[heap_size - 1]->src = src;
    min_heap[heap_size - 1]->dest = dest;
    min_heap[heap_size - 1]->weight = weight;
    min_heap_index = heap_size;
    while (min_heap_index > 0 &&
           // father.weight > child.weight
           min_heap[(min_heap_index - 1) / 2]->weight > min_heap[min_heap_index - 1]->weight) {
        swap(min_heap[(min_heap_index - 1) / 2], min_heap[min_heap_index - 1]);
        // child <- father
        min_heap_index = (min_heap_index - 1) / 2;
    }
}

unsigned long long sum = 0;

void delete_min(Min_heap_node *min_heap[], Graph_starting_node *graph_starting_array[]) {

    if (heap_size < 1) return;

    if (graph_starting_array[min_heap[0]->dest]->min_path > min_heap[0]->weight /*+ graph_starting_array[min_heap[0]->src]->min_path*/) {
        if (graph_starting_array[min_heap[0]->dest]->min_path != INFINITY)
            sum = sum - graph_starting_array[min_heap[0]->dest]->min_path;
        graph_starting_array[min_heap[0]->dest]->min_path =
                min_heap[0]->weight /*+ graph_starting_array[min_heap[0]->src]->min_path*/;
        sum = sum + graph_starting_array[min_heap[0]->dest]->min_path;
    }
    /*else {
        graph_starting_array[min_heap[0]->dest]->min_path = min_heap[0]->weight;
        sum = sum + graph_starting_array[min_heap[0]->dest]->min_path;
    }*/

    //min <- min_heap[0];
    min_heap[0] = min_heap[heap_size - 1]; // put the last one on top
    heap_size--; // TODO maybe free is memory problem
    min_heapify(min_heap, 0); // modify the min_heap
    //return;
}

int main(void) {

    char *command = malloc(MAXCOMMAND);
    unsigned int d, k;

    // get the first line (with d and k)
    command = fgets(command, MAXCOMMAND, stdin);
    //printf("%s", command);

    unsigned int space_pos = 0;
    while (command[space_pos] != ' ') space_pos++; // space_pos is at the position of the space_pos (starting from 0)

    char *num1 = malloc(space_pos);
    //char *num2 = malloc(10);
    num1 = strncpy(num1, command, space_pos);
    //num2 = strncpy(num2, command + space_pos + 1, strlen(command) - space_pos - 2);

    d = atoi(num1);
    //k = atoi(num2);

    free(num1);
    //free(num2);

    // read input
    //unsigned long *graphVector = malloc((d - 1) * sizeof(unsigned long));
    //unsigned long graphVector[d - 1];
    //unsigned long graph_starting_node[d][d - 1];
    Graph_starting_node *graph_starting_array[d];
    Graph_node *graph_node = NULL;
    Min_heap_node *min_heap[d];
    //Min_heap_node *min_heap_node = malloc(sizeof(Min_heap_node));


    char *dest = malloc(sizeof(unsigned long long) * 2);
    unsigned long weight;

    unsigned int src_node_index;
    unsigned int dest_node_index;
    temp_for_swap = malloc(sizeof(Min_heap_node));
    unsigned int dest_to_remember;
    unsigned long weight_to_remember;

    unsigned long line_index;
    unsigned long i;

    char finish = 'f';

    command = fgets(command, MAXCOMMAND, stdin);
    while (command != NULL) {

        if (command[0] == 'A') {


            for (src_node_index = 0; src_node_index < d; src_node_index++) { // vertical cycle

                // initializing the graph (vector of lists)
                graph_starting_array[src_node_index] = malloc(sizeof(Graph_starting_node));
                graph_starting_array[src_node_index]->index = src_node_index;
                //graph_starting_array[src_node_index].color = 'f';
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
                            while (line_index + i < strlen(command) && command[line_index + i] != ',') i++;
                            //char *dest = malloc(sizeof(unsigned long long) * 2);
                            dest = strncpy(dest, command + line_index, i);
                            //if (strncpy_s(dest, sizeof(dest), command + line_index, i) == 0)
                            weight = atoi(dest);
                        }
                        else weight = 0; // on the diagonal
                        //printf("%lu\n", weight);

                        // saving the graph in a vector of lists
                        if (weight != 0) {
                            graph_node = malloc(sizeof(Graph_node));
                            graph_node->dest = dest_node_index;
                            graph_node->weight = weight;
                            //graph_node->color = 'f';
                            //graph_node->pred
                            if (graph_starting_array[src_node_index]->graph_node == NULL) {
                                graph_node->next = NULL;
                                graph_starting_array[src_node_index]->graph_node = graph_node;
                            }
                            else {
                                graph_node->next = graph_starting_array[src_node_index]->graph_node;
                                graph_starting_array[src_node_index]->graph_node = graph_node;
                            }

                            // TODO a little optimization
                            // initializing the HEAP
                            if (src_node_index == 0) {
                                insert(min_heap, src_node_index, dest_node_index, weight);
                            }
                        }

                        dest_node_index++;
                    }
                }

                if (graph_starting_array[0]->graph_node == NULL) {
                    finish = 't';
                }
            } // finished saving the graph

            while (heap_size > 0) {

                // take the min of min_heap and search for adjacent nodes
                dest_to_remember = min_heap[0]->dest;
                weight_to_remember = min_heap[0]->weight;
                delete_min(min_heap, graph_starting_array);

                // putting all the adjacent nodes in the min_head
                if (graph_starting_array[dest_to_remember]->graph_node != NULL) {
                    do {
                        insert(min_heap, dest_to_remember, graph_starting_array[dest_to_remember]->graph_node->dest,
                               weight_to_remember + graph_starting_array[dest_to_remember]->graph_node->weight);
                        graph_starting_array[dest_to_remember]->graph_node = graph_starting_array[dest_to_remember]->graph_node->next;
                    } while (graph_starting_array[dest_to_remember]->graph_node != NULL);
                }

            }

        }

        printf("%llu\n", sum);
        command = fgets(command, MAXCOMMAND, stdin);
    }

    return 0;
}

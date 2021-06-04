//
// Created by chen on 04/06/2021.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

int heap_size = 0;
int pos_min = 0;

void min_heapify(Min_heap_node *min_heap[], int n) {
    // if child.weight < father.weight : min = child
    if ((n * 2 + 1) <= heap_size - 1 && min_heap[n * 2 + 1]->weight < min_heap[n]->weight) pos_min = n * 2 + 1;
    else pos_min = n; // else min = father
    // if child.weight <
    if ((n * 2 + 2) <= heap_size - 1 && min_heap[n * 2 + 2]->weight < min_heap[pos_min]->weight) pos_min = n * 2 + 2;

    if (pos_min != n) {
        swap(min_heap[n], min_heap[pos_min]);
        min_heapify(min_heap, pos_min);
    }
}

int min_heap_index = 0;

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

//unsigned long long sum = 0;

void delete_min(Min_heap_node *min_heap[]) {
    if (heap_size < 1) return;
    //min <- min_heap_node[0];
    min_heap[0] = min_heap[heap_size - 1]; // put the last one on top
    heap_size--;
    min_heapify(min_heap, 0); // modify the min_heap
    //return;
}

int main(void){
    temp_for_swap = malloc(sizeof(Min_heap_node));
    Min_heap_node *min_heap[8];

    insert(min_heap, 0, 2, 5);
    insert(min_heap, 0, 3, 4);
    insert(min_heap, 1, 2, 13);
    insert(min_heap, 1, 4, 3);
    insert(min_heap, 2, 3, 6);
    insert(min_heap, 3, 1, 4);
    insert(min_heap, 3, 4, 10);
    insert(min_heap, 3, 4, 10);
    insert(min_heap, 3, 4, 10);
    //insert(min_heap, 3, 4, 10);

    //delete_min(min_heap);
    //delete_min(min_heap);
    //delete_min(min_heap);
    //delete_min(min_heap);
    //delete_min(min_heap);
    //delete_min(min_heap);
    //delete_min(min_heap);
    //delete_min(min_heap);

    for (int i = 0; i < heap_size; ++i) {
        printf("s: %u, d: %u, w: %lu\n", min_heap[i]->src, min_heap[i]->dest, min_heap[i]->weight);
    }

}
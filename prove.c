//
// Created by chen on 29/05/2021.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct node {
    unsigned long sum;
    int graph_index;
    struct node *left;
    struct node *right;
    struct node *father;
} Ciao;

void printTree(Ciao *node) {
    if (node == NULL) return;

    printTree(node->left);
    printf("%d ", node->graph_index);
    printTree(node->right);

    /*if (node->left != NULL) {
        printTree(node->left);
    } else {
        printf("%d ", node->graph_index);
        return;
    }
    if (node->right != NULL) {
        printTree(node->right);
    } else {
        printf("&d ", node->graph_index);
        return;
    }*/
}

int main(void) {
    Ciao *root = malloc(sizeof(Ciao));
    Ciao *left1 = malloc(sizeof(Ciao));
    Ciao *right1 = malloc(sizeof(Ciao));
    //Ciao *left2 = malloc(sizeof(Ciao));
    //Ciao *right2 = malloc(sizeof(Ciao));

    root->graph_index = 69;
    root->father = NULL;
    root->right = right1;
    root->left = left1;
    left1->father = right1->father = root;
    left1->left = left1->right = right1->left = right1->right = NULL;
    left1->graph_index = 6;
    right1->graph_index = 9;

    printTree(root);


}
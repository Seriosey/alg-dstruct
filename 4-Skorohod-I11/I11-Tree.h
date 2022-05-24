#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define T 2

typedef struct node {
    struct node* parent;
    struct node* children[2 * T];
    struct node* brother;
    int length;
    int keys[2 * T-1];
} node;

node* create_node(int key, node* first, node* second, node* parent);
int is_leaf(node* v);
node* insert(node* v, int x);
void insert_to_node(node* v, int x);
node* search(node* v, int  x);
node* search_min(node* v);
int find(node* v, int x);
void sort(node* v);
node* delete(node* v, int x);
void remove_from_node(node* v, int x);
node* split(node* v);
node* merge(node* leaf);
void become_node2(node* n, int k, node* first, node* second);
node* fix(node* v);
node* redistribute(node* leaf);

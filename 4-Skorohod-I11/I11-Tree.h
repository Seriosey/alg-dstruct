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

node* Node();

node* insert(node* v, int x);

void insert_to_node(node* v, int x);

node* search(node* v, int  data);

node* search_min(node* v);

void sort(node* v);

node* delete(node* v, int x);

int get_height(node* v);

node* split(node* v);

node* merge(node* leaf);

node* fix(node* v);

node* redistribute(node* leaf);


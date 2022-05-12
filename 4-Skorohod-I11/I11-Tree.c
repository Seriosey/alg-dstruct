#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "I11-Tree.h"
#include <malloc.h>
#define T 2
#define _CRT_SECURE_NO_WARNINGS

node* create_node(int key, node* first, node* second, node* parent) {
    node* n = (node*)malloc(sizeof(node));
    n->length = 1;
    n->keys[0] = key;
    n->children[0] = first;
    n->children[1] = second;
    n->children[2] = NULL;
    n->parent = parent;
    n->children[3] = NULL;
    n->brother = NULL;
    return n;
}

void become_node2(node* n, int k, node* first, node* second) {
    n->keys[0] = k;
    n->children[0] = first;
    n->children[1] = second;
    n->children[2] = NULL;
    n->parent = NULL;
    n->brother = NULL;
    n->children[3] = NULL;
    n->length = 1;
}

int find(node* v, int x) {
    if (v == NULL)
        return 0;
    for (int i = 0; i < v->length; i++)
        if (v->keys[i] == x) return 1;
    return 0;
}

node* v;

node* root = NULL;

node* insert(node* v, int x) { 

    if (v == NULL) return create_node(x, NULL, NULL, NULL);
    if (is_leaf(v)) insert_to_node(v, x);
    else if (x <= v->keys[0]) insert(v->children[0], x);
    else if (v->length == 1 || v->length == 2 &&  x <= v->keys[1]) insert(v->children[1], x);
    else insert(v->children[2], x);

    return (node*)split(v);
}

node* split(node* v) {
    if (v-> length < 3) return (node*)v;

    node* x = create_node(v->keys[0], v->children[0], v->children[1], v->parent);
    node* y = create_node(v->keys[2], v->children[2], v->children[3], v->parent);

    x->brother = y;

    if (x->children[0]) x->children[0]->parent = x;
    if (x->children[1]) x->children[1]->parent = x;
    if (y->children[0]) y->children[0]->parent = y;
    if (y->children[1]) y->children[1]->parent = y;

    if (v->parent) {
        insert_to_node(v->parent, v->keys[1]);

        if (v->parent->children[0] == v) v->parent->children[0] = NULL;
        else if (v->parent->children[1] == v) v->parent->children[1] = NULL;
        else if (v->parent->children[2] == v) v->parent->children[2] = NULL;

        if (v->parent->children[0] == NULL) {
            v->parent->children[3] = v->parent->children[2];
            v->parent->children[2] = v->parent->children[1];
            v->parent->children[1] = y;
            v->parent->children[0] = x;
        }
        else if (v->parent->children[1] == NULL) {
            v->parent->children[3] = v->parent->children[2];
            v->parent->children[2] = y;
            v->parent->children[1] = x;
        }
        else {
            v->parent->children[3] = y;
            v->parent->children[2] = x;
        }

        return (node*)(v->parent);
    }
    else {
        x->parent = v;
        y->parent = v;
        become_node2(v, v->keys[1], x, y);
        return (node*)v;
    }
}

int is_leaf(node* v) {
    return (v->children[0] == NULL && v->children[1] == NULL && v->children[2] == NULL);
}

void insert_to_node(node* v, int x) {
    v->keys[v->length] = x;
    v->length+= 1;
    sort(v);
}

void remove_from_node(node* v, int k) {
    if (v->length >= 1 && v->keys[0] == k) {
        v->keys[0] = v->keys[1];
        v->keys[1] = v->keys[2];
        v->length -= 1;
    }
    else if (v->length == 2 && v->keys[1] == k) {
        v->keys[1] = v->keys[2];
        v->length -= 1;
    }
}

void sort(node* v) {
    if (v->length == 1) return;
    else if (v->length == 2) {
        if (v->keys[0] > v->keys[1]) {
            int temp = v->keys[0];
            v->keys[0] = v->keys[1];
            v->keys[1] = temp;
        }
    }
    else if (v->length == 3) {
        if (v->keys[0] > v->keys[1]) {
            int temp = v->keys[0];
            v->keys[0] = v->keys[1];
            v->keys[1] = temp;
        }
        if (v->keys[0] > v->keys[2]) {
            int temp = v->keys[0];
            v->keys[0] = v->keys[2];
            v->keys[2] = temp;
        }
        if (v->keys[1] > v->keys[2]) {
            int temp = v->keys[1];
            v->keys[1] = v->keys[2];
            v->keys[2] = temp;
        }
    }
}

node* search(node* v, int x) {
    if (!v) return NULL;
    if (find(v, x)) return v;
    else if (x < v->keys[0]) return search(v->children[0], x);
    else if ((v->length == 2) && (x < v->keys[1]) || (v->length == 1)) return search(v->children[1], x);
    else if (v->length == 2) return search(v->children[2], x);
    else return NULL;
}

node* delete(node* v, int x) {
    node* knode = search(v, x);
    if (knode == NULL) return v;

    node* min = NULL;
    if (knode->keys[0] == x)
        min = search_min(knode->children[1]);
    else
        min = search_min(knode->children[2]);

    if (min != NULL) {
        if (x == knode->keys[0]) {
            knode->keys[0] = min->keys[0];
            min->keys[0] = x;
        }
        else {
            knode->keys[1] = min->keys[0];
            min->keys[0] = x;
        }
        knode = min;
    }
    else return v;

    delete (knode, x);
    return fix(knode);
}

node* search_min(node* v) {
    if (!v) return NULL;
    if (!(v->children[0])) return v;
    else return search_min(v->children[0]);
}

node* fix(node* v) {
    if (v->length == 0 && v->parent == NULL)
        return NULL;

    if (v->length != 0) {
        if (v->parent) return fix(v->parent);
        else return v;
    }

    node* parent = v->parent;
    if (parent->children[0]->length == 2 || parent->children[1]->length == 2 || parent->length == 2)
        v = redistribute(v);
    else
        v = merge(v);

    return fix(v);
}

node* redistribute(node* leaf) {
    node* parent = leaf->parent;
    node* first = parent->children[0];
    node* second = parent->children[1];
    node* third = parent->children[2];
   
    if ((parent->length == 2) && (first->length < 2) && (second->length < 2) && (third->length < 2)) {
        if (first == leaf) {
            parent->children[0] = parent->children[1];
            parent->children[1] = parent->children[2];
            parent->children[2] = NULL;
            insert_to_node(parent->children[0], parent->keys[0]);
            parent->children[0]->children[2] = parent->children[0]->children[1];
            parent->children[0]->children[1] = parent->children[0]->children[0];

            if (leaf->children[0] != NULL) parent->children[0]->children[0] = leaf->children[0];
            else if (leaf->children[1] != NULL) parent->children[0]->children[0] = leaf->children[1];

            if (parent->children[0]->children[0] != NULL) parent->children[0]->children[0]->parent = parent->children[0];

            remove_from_node(parent, parent->keys[0]);
            free(first);
        }
        else if (second == leaf) {
            insert_to_node(first, parent->keys[0]);
            remove_from_node(parent, parent->keys[0]);
            if (leaf->children[0] != NULL) first->children[2] = leaf->children[0];
            else if (leaf->children[1] != NULL) first->children[2] = leaf->children[1];

            if (first->children[2] != NULL) first->children[2]->parent = first;

            parent->children[1] = parent->children[2];
            parent->children[2] = NULL;

            free(second);
        }
        else if (third == leaf) {
            insert_to_node(second, parent->keys[1]);
            parent->children[2] = NULL;
            remove_from_node(parent, parent->keys[1]);
            if (leaf->children[0] != NULL) second->children[2] = leaf->children[0];
            else if (leaf->children[1] != NULL) second->children[2] = leaf->children[1];

            if (second->children[2] != NULL)  second->children[2]->parent = second;

            free(third);
        }
    }
    else if ((parent->length == 2) && ((first->length == 2) || (second->length == 2) || (third->length == 2))) {
        if (third == leaf) {
            if (leaf->children[0] != NULL) {
                leaf->children[1] = leaf->children[0];
                leaf->children[0] = NULL;
            }

            insert_to_node(leaf, parent->keys[1]);
            if (second->length == 2) {
                parent->keys[1] = second->keys[1];
                remove_from_node(second, second->keys[1]);
                leaf->children[0] = second->children[2];
                second->children[2] = NULL;
                if (leaf->children[0] != NULL) leaf->children[0]->parent = leaf;
            }
            else if (first->length == 2) {
                parent->keys[1] = second->keys[0];
                leaf->children[0] = second->children[1];
                second->children[1] = second->children[0];
                if (leaf->children[0] != NULL) leaf->children[0]->parent = leaf;

                second->keys[0] = parent->keys[0];
                parent->keys[0] = first->keys[1];
                remove_from_node(first, first->keys[1]);
                second->children[0] = first->children[2];
                if (second->children[0] != NULL) second->children[0]->parent = second;
                first->children[2] = NULL;
            }
        }
        else if (second == leaf) {
            if (third->length == 2) {
                if (leaf->children[0] == NULL) {
                    leaf->children[0] = leaf->children[1];
                    leaf->children[1] = NULL;
                }
                insert_to_node(second, parent->keys[1]);
                parent->keys[1] = third->keys[0];
                remove_from_node(third, third->keys[0]);
                second->children[1] = third->children[0];
                if (second->children[1] != NULL) second->children[1]->parent = second;
                third->children[0] = third->children[1];
                third->children[1] = third->children[2];
                third->children[2] = NULL;
            }
            else if (first->length == 2) {
                if (leaf->children[1] == NULL) {
                    leaf->children[1] = leaf->children[0];
                    leaf->children[0] = NULL;
                }
                insert_to_node(second, parent->keys[0]);
                parent->keys[0] = first->keys[1];
                remove_from_node(first, first->keys[1]);
                second->children[0] = first->children[2];
                if (second->children[0] != NULL) second->children[0]->parent = second;
                first->children[2] = NULL;
            }
        }
        else if (first == leaf) {
            if (leaf->children[0] == NULL) {
                leaf->children[0] = leaf->children[1];
                leaf->children[1] = NULL;
            }
            insert_to_node(first, parent->keys[0]);
            if (second->length == 2) {
                parent->keys[0] = second->keys[0];
                remove_from_node(second, second->keys[0]);
                first->children[1] = second->children[0];
                if (first->children[1] != NULL) first->children[1]->parent = first;
                second->children[0] = second->children[1];
                second->children[1] = second->children[2];
                second->children[2] = NULL;
            }
            else if (third->length == 2) {
                parent->keys[0] = second->keys[0];
                second->keys[0] = parent->keys[1];
                parent->keys[1] = third->keys[0];
                remove_from_node(third, third->keys[0]);
                first->children[1] = second->children[0];
                if (first->children[1] != NULL) first->children[1]->parent = first;
                second->children[0] = second->children[1];
                second->children[1] = third->children[0];
                if (second->children[1] != NULL) second->children[1]->parent = second;
                third->children[0] = third->children[1];
                third->children[1] = third->children[2];
                third->children[2] = NULL;
            }
        }
    }
    else if (parent->length == 1) {
        insert_to_node(leaf, parent->keys[0]);

        if (first == leaf && second->length == 2) {
            parent->keys[0] = second->keys[0];
            remove_from_node(second, second->keys[0]);

            if (leaf->children[0] == NULL) leaf->children[0] = leaf->children[1];

            leaf->children[1] = second->children[0];
            second->children[0] = second->children[1];
            second->children[1] = second->children[2];
            second->children[2] = NULL;
            if (leaf->children[1] != NULL) leaf->children[1]->parent = leaf;
        }
        else if (second == leaf && first->length == 2) {
            parent->keys[0] = first->keys[1];
            remove_from_node(first, first->keys[1]);

            if (leaf->children[1] == NULL) leaf->children[1] = leaf->children[0];

            leaf->children[0] = first->children[2];
            first->children[2] = NULL;
            if (leaf->children[0] != NULL) leaf->children[0]->parent = leaf;
        }
    }
    return parent;
}

int get_height(node* v) {
    int h = 0;
    while (v->length != 1) {
        h++;
        v = v->children[0];
    }
    return h;
}

node* merge(node* leaf) {
    node* parent = leaf->parent;

    if (parent->children[0] == leaf) {
        insert_to_node(parent->children[1], parent->keys[0]);
        parent->children[1]->children[2] = parent->children[1]->children[1];
        parent->children[1]->children[1] = parent->children[1]->children[0];

        if (leaf->children[0] != NULL) parent->children[1]->children[0] = leaf->children[0];
        else if (leaf->children[1] != NULL) parent->children[1]->children[0] = leaf->children[1];

        if (parent->children[1]->children[0] != NULL) parent->children[1]->children[0]->parent = parent->children[1];

        remove_from_node(parent, parent->keys[0]);
        free(parent->children[0]);
        parent->children[0] = NULL;
    }
    else if (parent->children[1] == leaf) {
        insert_to_node(parent->children[0], parent->keys[0]);

        if (leaf->children[0] != NULL) parent->children[0]->children[2] = leaf->children[0];
        else if (leaf->children[1] != NULL) parent->children[0]->children[2] = leaf->children[1];

        if (parent->children[0]->children[2] != NULL) parent->children[0]->children[2]->parent = parent->children[0];

        remove_from_node(parent, parent->keys[0]);
        free(parent->children[1]);
        parent->children[1] = NULL;
    }

    if (parent->parent == NULL) {
        node* tmp = NULL;
        if (parent->children[0] != NULL) tmp = parent->children[0];
        else tmp = parent->children[1];
        tmp->parent = NULL;
        free(parent);
        return tmp;
    }
    return parent;
}

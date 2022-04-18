#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "I11-Tree.h"
#include <malloc.h>
#define _CRT_SECURE_NO_WARNINGS

int main()
{
    char cmd;
    int value;
    node* tree = NULL;
    node* s;
    while (fscanf(stdin, "%c %i", &cmd, &value))
    {
        switch (cmd)
        {
        case 'a':
            tree = add(tree, value);
            break;
        case 'r':
            tree = del(tree, value);
            break;
        case 'f':
            s = search(tree, value);
            if (s->keys[0] == value)
                printf("yes\n");
            else
                printf("no\n");
            break;

        }

    }
    return 0;
}
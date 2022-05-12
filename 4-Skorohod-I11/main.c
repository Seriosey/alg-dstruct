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
    node* v = NULL;
    while (fscanf(stdin, "%c %i", &cmd, &value))
    {
        switch (cmd)
        {
        case 'a':
            v = insert(v, value);
            break;
        case 'r':
            v = delete(v, value);
            break;
        case 'f':
            if (search(v, value))
                fprintf(stdout, "yes\n");
            else
                fprintf(stdout, "no\n");
            break;

        }

    }
    return 0;
}

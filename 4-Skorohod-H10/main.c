#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "AVLTree.h"
#include <malloc.h>

int main() {
	int value;
	char letter;
	node* ROOT = NULL;
	while (fscanf(stdin, "%c %i", &letter, &value)!=EOF)
		switch (letter) {
		case 'a':
			ROOT = insertel(ROOT, value);
			break;
		case 'r':
			ROOT = deletel(ROOT, value);
			break;
		case 'f':
			if (findel(ROOT, value))
				puts("yes");
			else
				puts("no");
			break;
		default:
			break;
		}
	}

	return 0;
}

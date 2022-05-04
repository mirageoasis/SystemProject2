#include <stdio.h>
#include <stdlib.h>
#include "struct.h"

extern STOCK_NODE *temp_array;
extern STOCK_NODE *tree_head;

int cmp(const void *first, const void *second);
int binary_tree_init();
STOCK_NODE *binary_tree_build(int start, int end);
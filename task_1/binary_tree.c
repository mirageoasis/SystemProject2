#include "binary_tree.h"

int cmp(const void *first, const void *second)
{
    return ((STOCK_NODE *)first)->ID > ((STOCK_NODE *)second)->ID;
}

STOCK_NODE *binary_tree_build(int start, int end)
{
    if (start > end)
        return NULL;
    STOCK_NODE *pnew = (STOCK_NODE *)malloc(sizeof(STOCK_NODE));

    int mid = (start + end) / 2;
    fprintf(stdout, "the id is %d!\n", temp_array[mid].ID);
    pnew->ID = temp_array[mid].ID;
    pnew->left_stock = temp_array[mid].left_stock;
    pnew->price = temp_array[mid].price;
    pnew->left = binary_tree_build(start, mid - 1);
    pnew->right = binary_tree_build(mid + 1, end);

    return pnew;
}

void binary_tree_init(int lines)
{
    qsort(temp_array, lines, sizeof(STOCK_NODE), cmp);

    for (int i = 0; i < lines; i++)
    {
        // fprintf(stdout, "sorted %d %d %d\n", temp_array[i].ID, temp_array[i].left_stock, temp_array[i].price);
    }

    tree_head = binary_tree_build(0, lines - 1);
    free(temp_array);
}

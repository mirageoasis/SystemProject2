#include "binary_tree_test.h"

int front = 0; // 큐 앞
int rear = 0;  // 큐 뒤
STOCK_NODE *Queue[10000];

void Enqueue(STOCK_NODE *ptr)
{
    Queue[++rear] = ptr;
}

STOCK_NODE *Dequeue()
{
    return Queue[++front];
}

void levelOrder(STOCK_NODE *ptr)
{
    if (!ptr)
        return;

    Enqueue(ptr);

    while (1)
    {
        ptr = Dequeue();
        if (ptr)
        {
            printf("%d ", ptr->ID);

            if (ptr->left)
                Enqueue(ptr->left);
            if (ptr->right)
                Enqueue(ptr->right);
        }
        else
            break;
    }
    fprintf(stdout, "\n");
}

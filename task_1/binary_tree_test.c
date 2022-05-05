#include "binary_tree_test.h"

int front = 0; // 큐 앞
int rear = 0;  // 큐 뒤

void Enqueue(STOCK_NODE *ptr, STOCK_NODE **Queue)
{
    // fprintf(stdout, "enqueue rear %d!\n", rear + 1);
    Queue[++rear] = ptr;
}

STOCK_NODE *Dequeue(STOCK_NODE **Queue)
{
    // fprintf(stdout, "dequeue! front %d\n", front + 1);
    return Queue[++front];
}

void levelOrder(STOCK_NODE *ptr, int lines)
{
    if (!ptr)
        return;

    STOCK_NODE **Queue = (STOCK_NODE **)malloc(sizeof(STOCK_NODE *) * (lines + 10));

    for (int i = 0; i < lines + 10; i++)
        Queue[i] = 0;

    // STOCK_NODE *Queue[lines]; // 이거 숫자 고치기

    Enqueue(ptr, Queue);

    while (1)
    {
        ptr = Dequeue(Queue);
        // fprintf(stdout, "front: %d rear: %d\n", front, rear);
        if (ptr)
        {
            assert(ptr != NULL);
            // fprintf(stdout, "%d\n", ptr->ID);

            if (ptr->left)
                Enqueue(ptr->left, Queue);
            if (ptr->right)
                Enqueue(ptr->right, Queue);
        }
        else
            break;
    }
    // fprintf(stdout, "queue finished\n");
    free(Queue);
}

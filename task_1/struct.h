#include <semaphore.h>

#ifndef STRUCT_H__
#define STRUCT_H__

typedef struct STOCK_NODE
{
    int ID;
    int left_stock;
    int price;
    int readcnt;
    sem_t mutex;
    struct STOCK_NODE *left;
    struct STOCK_NODE *right;
} STOCK_NODE;

#endif
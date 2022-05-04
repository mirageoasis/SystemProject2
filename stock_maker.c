#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_STOCK_ID 100 /*주식이 가질 수 있는 최대 ID*/
#define MAX_REMAIN 10    /*주식의 최대 수량*/
#define MAX_COST 10000   /*주식의 최대 가격*/
#define MAX_INFO 10      /*정보의 개수*/

int main()
{
    FILE *fp;

    fprintf(stdout, "welcome to random stock.txt maker!\n");

    if ((fp = fopen("stock.txt", "w")) == NULL)
    {
        fprintf(stdout, "file open failed!\n");
        return 0;
    }

    srand((unsigned int)time(NULL)); /*시간 초기화*/

    for (int i = 0; i < MAX_INFO; i++)
    {
        int stock_id = rand() % MAX_STOCK_ID + 1; /*주식 ID 설정*/
        int remain = rand() % MAX_REMAIN + 1;     /*주식 잔여 수량 설정*/
        int cost = rand() % MAX_COST + 1;         /*주식 가격 설정*/

        fprintf(fp, "%d %d %d\n", stock_id, remain, cost);
    }

    fclose(fp);
    fprintf(stdout, "random stock.txt maker finished!\n");
    return 0;
}
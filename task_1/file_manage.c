#include "file_manage.h"

void read_file()
{
    FILE *fp;
    int stock_id;
    int remains;
    int cost;

    if ((fp = fopen("stock.txt", "r")) == NULL)
    {
        fprintf(stdout, "file does not exists!\n");
        exit(0);
    }

    while (fscanf(fp, "%d %d %d", &stock_id, &remains, &cost) != EOF)
    {
        lines++;
    }
    // fprintf(stdout, "total line is! %d\n", lines);

    fclose(fp);

    if ((fp = fopen("stock.txt", "r")) == NULL)
    {
        fprintf(stdout, "file does not exists!\n");
        exit(0);
    }

    temp_array = (STOCK_NODE *)malloc(sizeof(STOCK_NODE) * lines); /*파일의 줄 수 만큼 주식 개수 생성*/

    for (int i = 0; i < lines; i++)
    {
        fscanf(fp, "%d %d %d", &temp_array[i].ID, &temp_array[i].left_stock, &temp_array[i].price);
        // fprintf(stdout, "stock %d info is :  %d %d %d\n", i + 1, temp_array[i].ID, temp_array[i].left_stock, temp_array[i].price);
    }

    fclose(fp);
}

/*
 * echoserveri.c - An iterative echo server
 */
/* $begin echoserverimain */
#include "csapp.h"
#include "struct.h"
#include "binary_tree.h"
#include "binary_tree_test.h"

STOCK_NODE *temp_array; /*처음에 파일 읽을 때 임시 저장하는 array*/
STOCK_NODE *tree_head;  /*이진 트리의 root node*/

int lines = 0; /*파일 라인 수(주식의 개수)*/

void echo(int connfd);

int read_file()
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
    fprintf(stdout, "total line is! %d\n", lines);

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

int main(int argc, char **argv)
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr; /* Enough space for any address */ // line:netp:echoserveri:sockaddrstorage
    char client_hostname[MAXLINE], client_port[MAXLINE];

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    read_file();

    // fprintf(stdout, "read file done!\n");

    // building binary tree
    binary_tree_init(lines); /*이진 트리 빌드하기(qsort로 배열 정렬하고 head 설정)*/

    // fprintf(stdout, "building tree done!\n");
    assert(tree_head != NULL);
    // fprintf(stdout, "head %d %d %d\n", tree_head->ID, tree_head->left_stock, tree_head->price);

    levelOrder(tree_head); // test code

    // exit(0);

    listenfd = Open_listenfd(argv[1]);
    while (1)
    {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *)&clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
        printf("Connected to (%s, %s)\n", client_hostname, client_port);
        echo(connfd);
        Close(connfd);
    }
    exit(0);
}
/* $end echoserverimain */

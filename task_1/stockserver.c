/*
 * echoserveri.c - An iterative echo server
 */
/* $begin echoserverimain */

#include "stockserver.h"

int lines = 0; /*파일 라인 수(주식의 개수)*/

void echo(int connfd);

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

    // levelOrder(tree_head); // test code

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

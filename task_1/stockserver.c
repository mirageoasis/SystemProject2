/*
 * echoserveri.c - An iterative echo server
 */
/* $begin echoserverimain */

#include "stockserver.h"

typedef struct
{ /* Represents a pool of connected descriptors */ // line:conc:echoservers:beginpool
    int maxfd;                                     /* Largest descriptor in read_set */
    fd_set read_set;                               /* Set of all active descriptors */
    fd_set ready_set;                              /* Subset of descriptors ready for reading  */
    int nready;                                    /* Number of ready descriptors from select */
    int maxi;                                      /* Highwater index into client array */
    int clientfd[FD_SETSIZE];                      /* Set of active descriptors */
    rio_t clientrio[FD_SETSIZE];                   /* Set of active read buffers */
} pool;                                            // line:conc:echoservers:endpool

int lines = 0;    /*파일 라인 수(주식의 개수)*/
int byte_cnt = 0; // 읽은 바이트수
sem_t readMutex;

void echo(int connfd);
void init_pool(int listenfd, pool *p);
void add_client(int connfd, pool *p);
int check_clients(pool *p);

int main(int argc, char **argv)
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr; /* Enough space for any address */ // line:netp:echoserveri:sockaddrstorage
    static pool pool;
    char client_hostname[MAXLINE], client_port[MAXLINE];
    int status = 0; // 주식 현재 상태

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

    // levelOrder(tree_head, lines); // test code
    //  fprintf(stdout, "levleorder fin!\n");
    //   exit(0);

    listenfd = Open_listenfd(argv[1]);
    init_pool(listenfd, &pool); // line:conc:echoservers:initpool
    while (1)
    {
        pool.ready_set = pool.read_set;
        fprintf(stdout, "before select!\n");
        pool.nready = Select(pool.maxfd + 1, &pool.ready_set, NULL, NULL, NULL);
        fprintf(stdout, "after select!\n");
        /* If listening descriptor ready, add new client to pool */
        if (FD_ISSET(listenfd, &pool.ready_set))
        { // line:conc:echoservers:listenfdready
            clientlen = sizeof(struct sockaddr_storage);
            connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen); // line:conc:echoservers:accept
            add_client(connfd, &pool);                                // line:conc:echoservers:addclient
        }
        /* Echo a text line from each ready connected descriptor */
        status = check_clients(&pool); // line:conc:echoservers:checkclients
        fprintf(stdout, "line 74!\n");
    }
    exit(0);
}

/* $begin init_pool */
void init_pool(int listenfd, pool *p)
{
    /* Initially, there are no connected descriptors */
    int i;
    p->maxi = -1; // line:conc:echoservers:beginempty
    for (i = 0; i < FD_SETSIZE; i++)
        p->clientfd[i] = -1; // line:conc:echoservers:endempty

    /* Initially, listenfd is only member of select read set */
    p->maxfd = listenfd; // line:conc:echoservers:begininit
    FD_ZERO(&p->read_set);
    FD_SET(listenfd, &p->read_set); // line:conc:echoservers:endinit
}
/* $end init_pool */

/* $begin add_client */
void add_client(int connfd, pool *p)
{
    int i;
    p->nready--;
    for (i = 0; i < FD_SETSIZE; i++) /* Find an available slot */
        if (p->clientfd[i] < 0)
        {
            /* Add connected descriptor to the pool */
            p->clientfd[i] = connfd;                 // line:conc:echoservers:beginaddclient
            Rio_readinitb(&p->clientrio[i], connfd); // line:conc:echoservers:endaddclient

            /* Add the descriptor to descriptor set */
            FD_SET(connfd, &p->read_set); // line:conc:echoservers:addconnfd

            /* Update max descriptor and pool highwater mark */
            if (connfd > p->maxfd) // line:conc:echoservers:beginmaxfd
                p->maxfd = connfd; // line:conc:echoservers:endmaxfd
            if (i > p->maxi)       // line:conc:echoservers:beginmaxi
                p->maxi = i;       // line:conc:echoservers:endmaxi
            break;
        }
    if (i == FD_SETSIZE) /* Couldn't find an empty slot */
        app_error("add_client error: Too many clients");
}
/* $end add_client */

/* $begin check_clients */
int check_clients(pool *p)
{
    int i, connfd, n;
    char buf[MAXLINE];       // 기존 입력
    char BUF2[MAXLINE];      // 복사본
    char clientBuf[MAXLINE]; // client 한테 보낼 입력
    char *argv[MAXLINE];
    rio_t rio;

    for (i = 0; (i <= p->maxi) && (p->nready > 0); i++)
    {
        connfd = p->clientfd[i];
        rio = p->clientrio[i];

        /* If the descriptor is ready, echo a text line from it */
        if ((connfd > 0) && (FD_ISSET(connfd, &p->ready_set)))
        {
            p->nready--;
            if ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0)
            {
                byte_cnt += n; // line:conc:echoservers:beginecho
                // printf("Server received %d (%d total) bytes on fd %d\n", n, byte_cnt, connfd);fw190d-9

                // fprintf(stdout, "%s", buf); // buf는 입력이다. 마지막에 개행문자 추가
                memset(clientBuf, 0, MAXLINE);
                strcpy(BUF2, buf);
                BUF2[strlen(BUF2) - 1] = 0;
                argvFeed(BUF2, argv);
                // argvTest(argv);
                if (1)
                {
                    if (!strcmp(argv[0], "show"))
                    {
                        // lock 걸기
                        show_binary_tree(tree_head, clientBuf, connfd);
                    }
                    else if (!strcmp(argv[0], "buy"))
                    {
                        // lock 걸기
                        fprintf(stdout, "in buy now!\n");

                        if (argv[1] == NULL || argv[2] == NULL)
                        {
                            sprintf(clientBuf, "PLEASE PUT THE RIGHT COMMAND!\n");
                        }

                        int id = atoi(argv[1]);     // 주식 id
                        int amount = atoi(argv[2]); // 구매할 주식 양

                        STOCK_NODE *target = binary_tree_search(id);

                        if (target == NULL)
                        {
                            sprintf(clientBuf, "stock not found!\n");
                        }
                        else
                        {
                            sem_wait(&target->write);
                            fprintf(stdout, "before sem_wait!\n");

                            if (amount > target->left_stock)
                            {
                                //재고보다 구매량이 많은 경우
                                sprintf(clientBuf, "Not enough left stock\n");
                            }
                            else
                            {
                                target->left_stock -= amount;
                                sprintf(clientBuf, "[buy] " ANSI_COLOR_GREEN "sucess\n" ANSI_COLOR_RESET);
                            }
                            fprintf(stdout, "before sem_post!\n");
                            sem_post(&target->write);
                        }
                    }
                    else if (!strcmp(argv[0], "sell"))
                    {
                        // lock 걸기
                        fprintf(stdout, "in sell now!\n");

                        if (argv[1] == NULL || argv[2] == NULL)
                        {
                            sprintf(clientBuf, "PLEASE PUT THE RIGHT COMMAND!\n");
                        }

                        int id = atoi(argv[1]);     // 주식 id
                        int amount = atoi(argv[2]); // 구매할 주식 양

                        STOCK_NODE *target = binary_tree_search(id);

                        if (target == NULL)
                        {
                            sprintf(clientBuf, "stock not found!\n");
                        }
                        else
                        {
                            sem_wait(&target->write);

                            target->left_stock += amount;                                               // 고객이 파는 상황이므로 주식의 개수를 늘린다.
                            sprintf(clientBuf, "[sell] " ANSI_COLOR_GREEN "sucess\n" ANSI_COLOR_RESET); //성공 출력메시지

                            sem_post(&target->write);
                        }
                    }
                    else if (!strcmp(argv[0], "exit"))
                    {
                        Close(connfd);                // line:conc:echoservers:closeconnfd
                        FD_CLR(connfd, &p->read_set); // line:conc:echoservers:beginremove
                        p->clientfd[i] = -1;          // line:conc:echoservers:endremove
                    }
                    else
                    {
                        fprintf(stdout, "wrong command!\n");
                    }
                }
                fprintf(stdout, "%s", clientBuf);
                Rio_writen(connfd, clientBuf, strlen(clientBuf)); // line:conc:echoservers:endecho
                // Rio_writen(connfd, buf, n);                       // line:conc:echoservers:endecho
                //  Rio_writen 공부하기
                /* EOF detected, remove descriptor from pool */
            }
            else
            {
                Close(connfd);                // line:conc:echoservers:closeconnfd
                FD_CLR(connfd, &p->read_set); // line:conc:echoservers:beginremove
                p->clientfd[i] = -1;          // line:conc:echoservers:endremove
            }
        }
        fprintf(stdout, "go for another connected descriptor!\n");
    }
    fprintf(stdout, "check function!\n");
    return 0;
}
/* $end check_clients */
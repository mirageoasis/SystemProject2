#include "csapp.h"
#include "struct.h"
#include "binary_tree.h"
#include "binary_tree_test.h"
#include "file_manage.h"

STOCK_NODE *temp_array; /*처음에 파일 읽을 때 임시 저장하는 array*/
STOCK_NODE *tree_head;  /*이진 트리의 root node*/
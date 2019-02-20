#ifndef __MODULE1_H__
#define __MODULE1_H__

#include "../../02-com/common.h"
#include "../../02-com/includes.h"
#include <ncurses.h>
#include <locale.h>
#include "Module2.h"


typedef struct _snakeNode
{
    int x , y ; //snakde的位置 xy
    char ch ; //以x代表一个节点 ,开头以>表示 
    struct _snakeNode *prev ; //Snake上一个节点
    struct _snakeNode *next ; //Snake下一个节点
}SnakeNode;

//以双向链表做为snake
typedef struct 
{
    SnakeNode *head; //头节点  
    int SnakeOfLength; //Snake长度
}Snake;

typedef struct {
    int x , y;
    BOOL IsEaten;
}Food;

typedef enum{
    up,
    down,
    left,
    right,
}SnakeMoveType;

Snake* InitSnakeNode(int starty,int startx);
SnakeNode *GenerateNode(int y,int x,BOOL IsHeadNode);
void AddNodeToTail(Snake*s ,SnakeNode *node, BOOL IsHeadFlag);
void Start_Game(void);
void CleanAllData(void);

/*  需要使用的全局变量 */
__WinAttri WindowAttr g_WindowOfGame;
__WinAttri WindowAttr g_WindowOfScore;
__StrAttri StringAttr g_StrOfScore ;

#endif
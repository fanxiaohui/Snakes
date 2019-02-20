/*
    贪吃蛇部分
*/

#include "Module1.h"
#include "Module2.h"
#include "my_thread.h"
#include "my_timer.h"

SnakeMoveType MoveOfState = right; //snake的移动状态
GameOfStateType g_GameOfState = Idle; //初始化游戏状态
Snake *g_SnakeHead ; // 蛇的头节点
Food *g_FoodState ; //食物信息


SnakeNode *GenerateNode(int y,int x,BOOL IsHeadNode)
{
    SnakeNode *node = (SnakeNode*)malloc(sizeof(SnakeNode));
    node->x = x , node->y = y;
    node->next = node->prev = node; //双向链表，指向自己
    if(IsHeadNode)
    {
        switch (MoveOfState)
        {
            case up:
                node->ch = '^';
                break;
            case down:
                node->ch = 'v';
                break;
            case left:
                node->ch = '<';
                break;
            case right:
                node->ch = '>';
                break;        
        }
    }
    else
        node->ch = 'x';
    return node;   
}

Snake* InitSnakeNode(int starty,int startx)
{
    Snake* snake = (Snake*)malloc(sizeof(Snake));
    SnakeNode *head ; 

    //初始化头节点
    head = GenerateNode(starty,startx,TRUE);
    snake->head = head;
    snake->SnakeOfLength = 1;

    return snake;
}

/******************************************************
                    双链表操作API
*******************************************************/
void AddNodeToTail(Snake*s ,SnakeNode *node,BOOL IsHeadFlag)
{
    //获取链表头节点
    SnakeNode *head = s->head;
    
    //如果增加的节点为null，直接退出
    if(node == NULL)
        return ;

    head->prev->next = node ;
    node->prev = head->prev;

    head->prev = node ;
    node->next = head ;
    
    if(IsHeadFlag == TRUE)
    {
        head->ch = 'x';
        s->head = node ; //将node变为根节点,即从头节点增加
    }
        
    //蛇的长度+1
    s->SnakeOfLength ++ ;
}

void DeleteTailNode(void)
{
    SnakeNode *head = g_SnakeHead->head;
    SnakeNode *tail = head->prev;
    if(tail == head)
        return ;

    win_printw(g_WindowOfGame.win,tail->y,tail->x," "); //将尾节点从屏幕清除

    head->prev = tail->prev ;
    tail->prev -> next = head;

    free(tail);
    tail = NULL;
    g_SnakeHead->head = head;
    g_SnakeHead->SnakeOfLength--;
}

void DeleteDoubleLink(void)
{
    SnakeNode *node = g_SnakeHead->head,*tmp;
    int i ;
    for(i = 0 ; i < g_SnakeHead->SnakeOfLength;i++)
    {
        if(node->next==NULL)
            break;
        tmp = node ->next;
        free(node);
        node = NULL;
        node = tmp;
    }
}
/***************************************************
 *                     贪吃蛇动态API
****************************************************/

Food* GenerateFood(void)
{
    int edge_x = g_WindowOfGame.x ,edge_y = g_WindowOfGame.y;
    Food *food = (Food*)malloc(sizeof(Food));
    int x = 0 , y = 0;
    while(x == 0)x = rand() % edge_x;
    while(y == 0)y = rand() % edge_y;
    food->x = x ;//生成的food x不能在边界x与大于边界
    food->y = y ;
    food->IsEaten = FALSE;
    mvwprintw(g_WindowOfGame.win,food->y,food->x,"O");
    wrefresh(g_WindowOfGame.win);
    return food; 
}

void SnakeEatFood(void)
{
    SnakeNode *tail = g_SnakeHead->head->prev; //获取尾节点
    SnakeNode *prev = tail->prev ;
    SnakeNode *node ; //新节点
    //获取尾节点与尾节点前面的xy，用来计算新增加的节点的xy
    int tail_x = tail->x , tail_y = tail->y;  
    int prev_y = prev->y;  
    int x ,y ;
    if(tail_y == prev_y)
    {
        x = tail_x + 1 ;
        y = tail_y;
    }
    else
    {
        x = tail_x ;
        y = tail_y + 1;
    }
    node = GenerateNode(y,x,FALSE);
    AddNodeToTail(g_SnakeHead,node,FALSE);
}
void UpdateScore(void)
{
    WINDOW * win = g_StrOfScore.win ;
    int y = g_StrOfScore.y , x = g_StrOfScore.x;
    int *Score ;
    
    Score = &g_StrOfScore.score ;
    mvwprintw(win,y,x,"%d",++(*Score) );
    wrefresh(win);
}
int isEatFood(void)
{
    int x , y; //食物的xy

    //判断是否被吃了
    if(g_FoodState->IsEaten == FALSE)
    {
        x = g_FoodState->x; y = g_FoodState->y;
        if(g_SnakeHead->head->x == x && g_SnakeHead->head->y == y )
        {
            SnakeEatFood();
            win_printw(g_WindowOfGame.win,y,x," "); //刷新界面,将食物吃掉
            UpdateScore();
            g_FoodState = GenerateFood();                           
        }
    }        
    return 1 ;
}

int isHitWall(void)
{
    SnakeNode *node = g_SnakeHead->head;
    int x= node->x ,y = node->y ;
    int edge_x = g_WindowOfGame.x ,edge_y = g_WindowOfGame.y;
    
    if(x == 0 || x == edge_x-1 || y == 0 || y == edge_y-1)
    {     
        g_GameOfState = Stop;       
        return -1;             
    }
    return 0;
    
}

int isTouchSelf(void)
{
    SnakeNode *node ,*head;//= g_SnakeHead->head;
    int len ,i;
    int x,y,head_x,head_y ;
    int CurrentMenuPageIndex = GetCurrentMenuPageIndex();
    
    head = g_SnakeHead->head;
    head_x = head->x ;head_y = head->y;
    node = head->next;
    len = g_SnakeHead->SnakeOfLength ;

    for(i =1 ;i < len ;i++)
    {
        if(CurrentMenuPageIndex == MainMenu)
            return 0;
        x = node->x , y = node->y ;
        if(x == head_x && y == head_y)
        {
            g_GameOfState = Stop;
            return -1;
        }  
        node = node->next;       
    }
    return 0;
}

void SnakeRefresh(void)
{
    int i ;
    SnakeNode* node = g_SnakeHead->head;
    BOOL QuitFlag = FALSE;
    if( isEatFood() && (isHitWall() == -1 || isTouchSelf() == -1) )    
         QuitFlag = TRUE; 

    for(i = 0 ; i < g_SnakeHead->SnakeOfLength;i++)
    {     
        if(QuitFlag == TRUE)
        {
            QuitFlag = FALSE;
            node = node ->next ; i = 1;
        }
        mvwprintw(g_WindowOfGame.win,node->y,node->x,"%c",node->ch);
        node = node->next;        
    }

    wrefresh(g_WindowOfGame.win);
    Delay_x(100,ms);  
          
}


void* SnakeMoveDir(void *args)
{
    SnakeNode *head , *node;
    int y , x, CurrentMenuPageIndex ;
    
    while(1)
    {
        CurrentMenuPageIndex = GetCurrentMenuPageIndex();
        if(g_GameOfState == Stop || CurrentMenuPageIndex == MainMenu)
        {    
            if(g_GameOfState == Stop)
                GameOver_Win();                     
            return (void*)0;
        }
            
        head = g_SnakeHead->head;
        y = head->y, x= head->x;
        switch (MoveOfState)
        {
            case up:
                y--;           
                break;
            case down:
                y++;
                break;
            case left:
                x--;
                break;
            case right:
                x++;
                break;
        }
        node = GenerateNode(y,x,TRUE);
        AddNodeToTail(g_SnakeHead,node,TRUE);
        DeleteTailNode();
        SnakeRefresh();
    }
    return (void*)0;
}
extern MenuPageIndex g_CurrentMenuPageIndex;
void Start_Game(void)
{
    int edge_y = g_WindowOfGame.y;
    int startx = 1 , starty = edge_y / 2; //贪吃蛇开始的位置
    pthread_t Snake_Move ;
    MoveOfState = right;
    g_GameOfState = Gaming;
    g_CurrentMenuPageIndex = GameMenu;
    //初始化SNAKE
    g_SnakeHead = InitSnakeNode(starty,startx); 
    g_FoodState = GenerateFood();

    pthread_create(&Snake_Move,NULL,&SnakeMoveDir,NULL);
}

void CleanAllData(void)
{
    g_StrOfScore.score = 0;
    g_GameOfState = Idle;
    DeleteDoubleLink();
    free(g_FoodState );
}
#ifndef __MODULE2_H__
#define __MODULE2_H__

#include "../../02-com/common.h"
#include "../../02-com/includes.h"
#include <ncurses.h>
#include <locale.h>
#include <menu.h>

#define KEY_ESC 27

typedef INT16 PosInfo;

#define __MenuType      extern 
#define __WinAttri      extern 
#define __StrAttri      extern 

#define MenuPageIndex   UINT16      
#define NewRow()    printf("\n")
enum{
    MainMenu,
    GameMenu,
}MenuType;

typedef struct{
    WINDOW *win;
    int x , y ; 
    int startx,starty;
}WindowAttr; //窗口的信息

typedef struct 
{
    WINDOW *win ; //文字所在窗口
    int score ;   //分数
    int x , y ;   //文字起始位置
}StringAttr; //重要的文字信息

typedef enum 
{
    Idle,   //空闲，即主界面
    Gaming, //游戏中
    Pause,  //游戏暂停
    Resume, //游戏重新开始
    Stop,   //游戏结束
    
}GameOfStateType;

WINDOW *create_newwin(int height, int width, int starty, int startx,char v ,char h);

void init_curses(void);
void Main_menu(void);
void scan_key(void);
void game_menu(void);
void Illustration_Win(void);
void exit_menu(void);
void GameOver_Win(void);

/*                  通用API                 */
void GetWindowAttriVal(WindowAttr wa,WINDOW * win ,int *y,int *x , int *starty ,int *startx);
void GetStrAttriVal(StringAttr wa,WINDOW * win ,int *y,int *x , int *score);

int  GetCurrentMenuPageIndex(void);
void SaveWindowInfo(WindowAttr *info , WINDOW*win,int y,int x ,int starty,int startx);
void SaveStrInfo(StringAttr *info , WINDOW*win,int y,int x ,int score);
void win_printw(WINDOW*win,int y,int x ,const char *str);
#endif
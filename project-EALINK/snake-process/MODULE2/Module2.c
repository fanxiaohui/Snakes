/*
    背景部分
*/
#include "Module2.h"
#include "Module1.h"
#include "my_thread.h"

/*===============================================================
                        通用API
=================================================================*/
MenuPageIndex g_CurrentMenuPageIndex ;
extern GameOfStateType g_GameOfState;

int GetCurrentMenuPageIndex(void)
{
    return g_CurrentMenuPageIndex;
}

void SaveWindowInfo(WindowAttr *info , WINDOW*win,int y,int x ,int starty,int startx)
{
    info->x = x , info->y = y ;
    info->startx = startx ,info->starty = starty;
    info->win = win;
}

void SaveStrInfo(StringAttr *info , WINDOW*win,int y,int x ,int score)
{
    info->x = x , info->y = y ;
    info->score = score;
    info->win = win;
}

void GetWindowAttriVal(WindowAttr wa,WINDOW * win ,int *y,int *x , int *starty ,int *startx)
{
    win = wa.win; *y = wa.y ; *x = wa.x; *startx = wa.startx ; *starty = wa.starty;
}
void GetStrAttriVal(StringAttr wa,WINDOW * win ,int *y,int *x , int *score)
{
    win = wa.win; *y = wa.y ; *x = wa.x;  *score = wa.score;
}

void win_printw(WINDOW*win,int y,int x ,const char *str)
{
    mvwprintw(win,y,x,str);
    wrefresh(win);
}
/*===============================================================
                        窗口API
=================================================================*/
void draw_box(void)
{
    clear();
    refresh();
    box(stdscr,0,0);
    refresh();
}

void clear_screen(void)
{
    clear();
    refresh();
}
/*
    height : y
    width  : x
*/
WINDOW *create_newwin(int height, int width, int starty, int startx,char v ,char h)
{	
    WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
    if(v == 0 && h == 0)
        box(local_win,0,0);
    else
	    wborder(local_win, v,v,h,h,v,v,v,v);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/
	return local_win;
}

void destroy_win(WINDOW *local_win)
{	
	/* box(local_win, ' ', ' '); : This won't produce the desired
	 * result of erasing the window. It will leave it's four corners 
	 * and so an ugly remnant of window. 
	 */
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	/* The parameters taken are 
	 * 1. win: the window on which to operate
	 * 2. ls: character to be used for the left side of the window 
	 * 3. rs: character to be used for the right side of the window 
	 * 4. ts: character to be used for the top side of the window 
	 * 5. bs: character to be used for the bottom side of the window 
	 * 6. tl: character to be used for the top left corner of the window 
	 * 7. tr: character to be used for the top right corner of the window 
	 * 8. bl: character to be used for the bottom left corner of the window 
	 * 9. br: character to be used for the bottom right corner of the window
	 */
	wrefresh(local_win);
	delwin(local_win);
}



/*===============================================================
                        初始化API
=================================================================*/
void init_curses(void)
{
    //避免中文乱码
    setlocale(LC_ALL,"");
    //启动 curses模式
    initscr(); 
    //除了DELETE 与 CTRL等特殊字符以外，都立马被读取
    cbreak();
    //取消按下enter为'\n'
    nonl();
    //关闭字符显示在屏幕上
    noecho();
    //应对中断（break）反应较慢
    intrflush(stdscr,FALSE);
    //会将特殊按键如上下左右转化为KEY_xxx
    keypad(stdscr,TRUE);
    //隐藏光标：0
    curs_set(0);
    //刷新屏幕
    refresh(); 
} 

/*****************************************************
 *                      界面API
******************************************************/
WindowAttr g_WindowOfGame;
WindowAttr g_WindowOfScore;

StringAttr g_StrOfScore ;
void Main_menu(void)
{
    int x = (COLS / 4 -1);

    g_CurrentMenuPageIndex = MainMenu;
    //画出整体边框
    draw_box();
    
    //设置字体变粗
    attrset(A_NORMAL);
    attron(A_BOLD);
    mvprintw(LINES/4,COLS/2-3,"贪吃蛇Snake");
    attroff(A_BOLD);

    mvprintw(LINES-10,x,"1.开始游戏");
    mvprintw(LINES-10,x*2,"2.游戏说明");
    mvprintw(LINES-10,x*3,"3.退出游戏");

    mvprintw(LINES-5,x*2,"请选择：");
    curs_set(1);
    refresh();
}

void exit_menu(void)
{
    draw_box();

    curs_set(0);
    start_color();
    init_pair(1,COLOR_RED,0);
    attron(COLOR_PAIR(1));
    mvprintw(LINES/2,COLS/2-3,"退出游戏!");
    mvprintw(LINES/2+1,COLS/2-6,"请按任意按钮退出");
    attroff(COLOR_PAIR(1));
    refresh();
    getch();
}



void game_menu(void)
{  
    int x = COLS /3 *2 , y = LINES-5;
    static BOOL FirstFlag = TRUE;

    //游戏框，需要记录xy
    draw_box();
    g_WindowOfGame.win = create_newwin(y,x,2,2,0,0);
    
    if(FirstFlag)
    {
        getmaxyx(g_WindowOfGame.win,g_WindowOfGame.y,g_WindowOfGame.x);
        g_WindowOfGame.startx = g_WindowOfGame.starty = 2;
        FirstFlag = FALSE;
    }
    
    Illustration_Win(); //创建右边的说明框
    Start_Game();   //开始游戏
}

void Illustration_Win(void)
{
    WINDOW *win ;
    int x = g_WindowOfGame.x, y = g_WindowOfGame.y;
    
    win = create_newwin(y,COLS-x-3,g_WindowOfGame.starty,g_WindowOfGame.x+2,'*','*');
    
    //游戏说明 与 得分等
    getmaxyx(win,y,x);
    
    //将x分为二等分，y分为八等分
    y = y /8 ,x = x /2;  
    mvwprintw(win,y,x-5,"游戏操作说明");
    mvwprintw(win,y+3,x-6,"w : 上  s : 下");
    mvwprintw(win,y+4,x-6,"a : 左  d : 右");
    mvwprintw(win,y+6,x-9,"F1 : 加速  F2 : 减速");
    mvwprintw(win,y+10,x-3,"得分 : ");
    mvwprintw(win,y+10,x+3,"0");
    wrefresh(win);
    
    //将信息框保存起来
    SaveStrInfo(&g_StrOfScore,win,y+10,x+3,0);
    curs_set(0);
}



void GameOver_Win()
{
    int x = g_WindowOfGame.x / 3, y = g_WindowOfGame.y / 2 - 6;
    int max_y = 10 ,max_x = 40;

    WINDOW *win = create_newwin(max_y,max_x,y,x,0,0);
    mvwprintw(win,4,5,"游戏结束，请按c键返回主菜单");
    g_GameOfState = Idle;  
    wrefresh(win);
    CleanAllData();
}

/*****************************************************
 *                      键盘API
*****************************************************/
//使用一个线程来扫描按键

extern SnakeMoveType MoveOfState ; 
BOOL ChangeMainFlag ;
void *EnterOtherMenu(void *args)
{
    ChangeMainFlag = FALSE;
    while(1)
    {
        if(ChangeMainFlag == TRUE)
        {
            ChangeMainFlag = FALSE; 
            
            if(g_CurrentMenuPageIndex == GameMenu)
                game_menu(); 
            else if(g_CurrentMenuPageIndex == MainMenu)
                Main_menu();           
        }
    }
}
void* scan(void *args)
{
    char ch ;
    int menu ;
    
    while(1)
    {
        menu = GetCurrentMenuPageIndex();
        ch = getch();
        if(menu == MainMenu)
        {
            switch (ch)
            {
                case '1':
                    g_CurrentMenuPageIndex = GameMenu;
                    ChangeMainFlag = TRUE;                
                    break;
                case '2':
                    break;
                case '3':
                    exit_menu();
                    return (void*)0;                
            }
        }
        else if(menu == GameMenu)
        {
           switch (ch)
            {
                case 'w':
                    MoveOfState = up;
                    break;

                case 's':
                    MoveOfState = down;
                    break;

                case 'a':
                    MoveOfState = left;
                    break;

                case 'd':
                    MoveOfState = right;
                    break;

                case 'c':
                    g_CurrentMenuPageIndex = MainMenu;
                    ChangeMainFlag = TRUE; 
                    break;
            } 
        }
    }   
   // return (void*)0;
}

void scan_key(void)
{
    TaskHandle_t key_misson,entermenu;
    A_OSTaskCreate(&scan,NULL,NULL,&key_misson,NULL,NULL);
    A_OSTaskCreate(&EnterOtherMenu,NULL,NULL,&entermenu,NULL,NULL);
    pthread_join(key_misson,NULL);
    return ;
}
#include "Module2.h"
#include "Module1.h"

int main(int argc, char *argv[])
{	
    init_curses();
    
    Main_menu();  
    scan_key(); 
	endwin();			/* End curses mode		  */
	return 0;
}



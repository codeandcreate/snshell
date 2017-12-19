/*
	user interface
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/ioctl.h>

char *HEADERINFO1 = "SIMPLE NETWORK SHELL TERMINAL LINK\n";
char *HEADERINFO2 = "BUILD 0.1 - 19.12.2012\n";
char *HEADERINFO3 = "-Server 1-\n";

/* fallout ones: */

//char *HEADERINFO1 = "ROBCO INDUSTRIES UNIFIED OPERATING SYSTEM\n";
//char *HEADERINFO2 = "COPYRIGHT 2075-2077 ROBCO INDUSTRIES\n";
//char *HEADERINFO3 = "-Server 1-\n";


void snshell_print_header(int fast) 
{   
    printCenteredText(HEADERINFO1, fast);
    printCenteredText(HEADERINFO2, fast);
    printCenteredText(HEADERINFO3, fast);
    printText("\n", fast);    
}

int snshell_getTerminalSize(int type) 
{
    int cols = 80;
    int lines = 24;
    
    #ifdef TIOCGSIZE
        struct ttysize ts;
        ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
        cols = ts.ts_cols;
        lines = ts.ts_lines;
    #elif defined(TIOCGWINSZ)
        struct winsize ts;
        ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
        cols = ts.ws_col;
        lines = ts.ws_row;
    #endif /* TIOCGSIZE */
    
    switch (type) {
        case 0:
            return lines;
            break;
        case 1:
            return cols;
            break;
        default:
            return -1;
    }
}

void snshell_cls()
{   
    int i;
    for (i = 0; i != snshell_getTerminalSize(0); i++) {
        printText("\n", 1);
    }
}
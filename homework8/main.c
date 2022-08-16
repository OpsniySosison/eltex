#include "terminal.h"

int main(int argc, char* args[]) {
    
    FILE* file = fopen(args[1], "r+");
    if (!file) {
        fprintf(stderr, "File not found!\n");
        exit(EXIT_SUCCESS);
    }

    BUFF* buff = (BUFF*)malloc(sizeof(BUFF));
    CURS* curs = (CURS*)malloc(sizeof(CURS));

    BufferInit(file, buff);
    CursorInit(curs);

    initscr();
    noecho();
    keypad(stdscr, TRUE);
    OutputToTerminal(buff);
    move(curs->y, curs->x);

    while(1) { 
        int key = wgetch(stdscr);
        if (!KeyProcess(curs, buff, file, key)) {
            WriteToBuffer(curs, buff, key, GetCollumn(buff, curs) + curs->x);
            OutputToTerminal(buff);
            move(curs->y, curs->x);
        }
        refresh();
    }

    endwin();
    exit(EXIT_SUCCESS);
}
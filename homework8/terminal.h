
#include <ncurses.h>

#include "buffer.h"

int CursorInit(CURS* cursor);
int KeyProcess(CURS* cursor, BUFF* buff, FILE* file, int key);
int OutputToTerminal(const BUFF* buff);
unsigned int GetCollumn(BUFF* buff, CURS* cursor);
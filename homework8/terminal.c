#include "terminal.h"

int CursorInit(CURS* cursor) {
    cursor->x = 0;
    cursor->y = 0;
}

unsigned int GetCollumn(BUFF* buff, CURS* cursor) {
    unsigned int sum = 0;

    for (unsigned int i = 0; i < cursor->y; i++) {
        sum += buff->sizes_of_prev_strs[i];
    }
    
    return sum;
}


static int LenghtPrevStrs(BUFF* buff, CURS* cursor) {
    int i = GetCollumn(buff, cursor) + cursor->x;
    int lenght = 0;
    while ((i > 0) && (buff->str[i] != '\n')) {
        i--;
        lenght++;
    }

    return lenght;
}

int KeyProcess(CURS* cursor, BUFF* buff, FILE* file, int key) {
    switch (key) {
    case KEY_LEFT:
        cursor->x--;
        move(cursor->y, cursor->x);
        return BAD;
    case KEY_RIGHT:
        cursor->x++;
        move(cursor->y, cursor->x);
        return BAD;
    case KEY_UP:
        cursor->y--;
        move(cursor->y, cursor->x);
        return BAD;
    case KEY_DOWN:
        cursor->y++;
        move(cursor->y, cursor->x);
        return BAD;
    case KEY_BACKSPACE:
        buff->str[GetCollumn(buff, cursor) + cursor->x - 1] = ' ';
        cursor->x--;
        OutputToTerminal(buff);
        move(cursor->y, cursor->x);
        return BAD;
    case 0x0a:
        buff->sizes_of_prev_strs[cursor->y] = LenghtPrevStrs(buff, cursor);
        return NORMAL;
    case 0x1b:
        UploadToFile(buff, file);
        BufferDestructor(buff);
        endwin();
        exit(EXIT_SUCCESS);
    default:
        return NORMAL;
    }
}

int OutputToTerminal(const BUFF* buff) {
    clear();
    printw("%s", buff->str);
}

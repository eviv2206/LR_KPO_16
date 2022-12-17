#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <unistd.h> // usleep
#include <time.h> // for time

using namespace std;

const unsigned char BLACK = 219;

/* type of each pixel */
typedef struct {
    unsigned char elem;
    unsigned char color;
} SCREENELEMENT;

BOOL gotoxy(const WORD x, const WORD y) {
    COORD xy;
    xy.X = x;
    xy.Y = y;
    return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), xy);
}

/* print the screen to console */
inline
void printScreen(int width, int height, SCREENELEMENT **screen) {
    gotoxy(1, 1); // top-left corner
    for (int i = 0; i < height; i ++) {
        for (int j = 0; j < width; j ++) {
            SCREENELEMENT *cur = &screen[i][j];
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cur->color);
            printf("%c", cur->elem); // may be faster than cout
        }
    }
}

/* initialize the screen to element se */
inline
void initScreen(int width, int height, SCREENELEMENT **screen, SCREENELEMENT se) {
    for (int i = 0; i < height; i ++) {
        for (int j = 0; j < width; j ++) {
            screen[i][j] = se;
        }
    }
}

/* draw a rectangle to (x, y), size (szx, szy) */
inline
void putRectangle(int x, int y, int szx, int szy, int color, int height, SCREENELEMENT **screen) {
    for (int i = y; i < y + szy; i ++) {
        for (int j = x; j < x + szx; j ++) {
            screen[i][j].color = color;
        }
    }
}

int main() {
    SetConsoleOutputCP(437);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int ret;
    /* get the width of the console */
    ret = GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int width = 80;
    if (ret) {
        width = csbi.dwSize.X;
    }
    int height = csbi.srWindow.Bottom - csbi.srWindow.Top;
    SCREENELEMENT **s = new SCREENELEMENT*[height];
    for (int i = 0; i < height; i ++) {
        s[i] = new SCREENELEMENT[width];
    }
    SCREENELEMENT se;
    se.elem = BLACK;
    se.color = 0;
    initScreen(width, height, s, se);
    se.color = 15;
    srand(time(NULL));
    for (;;) {
        int x = rand() % width;
        int y = rand() % height;
        int szx = rand() % (width - x);
        int szy = rand() % (height - y);
        /* color from 0 to 15 */
        int c = rand() % 15 + 1;
        /* put a random rectangle */
        putRectangle(x, y, szx, szy, c, height, s);
        printScreen(width, height, s);
        usleep(20); // optional interval
    }
    for (int i = 0; i < height; i ++) {
        delete[] s[i];
    }
    delete[] s;
    return 0; /* use Ctrl+C, never returns */
}
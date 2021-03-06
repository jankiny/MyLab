/* Notice: don't use "cvas_x, cvas_y, range_x, canvas, mode" as variable name */

/* Principles: First, create a Canvas(define a rectangular space),   
 * which was filled with ' '(space), and surranded by black lines. 
 * Then, draw the Canvas with character and cursor. 
 */

/* Direction, x is pointing to down, and y is pointing to right */

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#ifndef __CURSOR_H__
#define __CURSOR_H__
#define SPACE ' '

int cvas_x, cvas_y;    //cursors
struct cursor {
    int x;
    int y;
} curs;
struct range {    // range of canvas
    int x;
    int y;
} range;
char **cvas;
int mode = 1; // 1(debug):show detials; 2(use): hide detials;

// create a Canvas
char **canvas(int a, int b) {
    int i, j;

    cvas = (char **)malloc((a+2)*sizeof(char *));
    for(i = 0; i < a+2; i++)
        cvas[i] = (char *)malloc((b+2)*sizeof(char));

    range.x = a+1;
    range.y = b+1;
    for(i = 0; i <= a+1; i++) {
        for(j = 0; j <= b+1; j++) {
            curs.x = i;
            curs.y = j;
            if(i == 0 || i == a+1)
                cvas[i][j] = '-';
            else if(j == 0 || j == b+1)
                cvas[i][j] = '|';
            else
                cvas[i][j] = ' ';
        }
    }
    return cvas;
}

void addch(char ch) {
    extern char **cvas;

    cvas[curs.x][curs.y] = ch;
}

void cursor_move(int a, int b) {
    if(a == 0 || b == 0 || a >= range.x || b >= range.y) {
        printf("cursor move out of range! \n");
        printf("(%d, %d) in range(1-%d,1-%d) \n", a, b, range.x, range.y);
        exit(1);
    }
    curs.x = a;
    curs.y = b;
}

// Notice: before use it, use cursor_move to right position(curs.x, curs.y)
void char_move(int a, int b) {
    char temp;

    if(a == 0 || b == 0 || a >= range.x || b >= range.y) {
        printf("character move out of range! \n");
        printf("(%d, %d) in range(1-%d,1-%d) \n", a, b, range.x, range.y);
        exit(1);
    }
    temp = cvas[curs.x][curs.y];
    cvas[curs.x][curs.y] = SPACE;
    curs.x = a;
    curs.y = b;
    cvas[curs.x][curs.y] = temp;
}

void refresh() {
    int i, j, t;

    system("clear");
    for(i = 0; i <= range.x; i++) {
        for(j = 0; j <= range.y; j++) {
            printf("%c", cvas[i][j]);
        }
        if(mode == 1) {
            printf("%2d", i);
        }
        putchar('\n');
    }
    if(mode == 1) {
        for(j = 0; j <= range.y; j += 2) {
            t = j%10;
            printf("%d ", t);
        }
        putchar('\n');
    }
}

void mode_change(int n) {
    if(n >= 1 && n <= 2) {
        mode = n;
    }
    else
        printf("Only 1-2 modes exist!\n");
}

int getch() {
    int c = 0;
    struct termios org_opts, new_opts;
    int res = 0;

    res = tcgetattr(STDIN_FILENO, &org_opts);
    assert(res==0);
    memcpy(&new_opts, &org_opts, sizeof(new_opts));
    new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
    c = getchar();
    res = tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
    assert(res == 0);
    return c;
}

#endif

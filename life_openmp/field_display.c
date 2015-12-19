#include <ncurses.h>
#include <string.h>
#include "two_dim_array_operations.h"

    
WINDOW* win;

void init_curses() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, true);
    curs_set(0);
}

void print_message_center(WINDOW* win, int line, char* buf)
{
    mvwaddstr(win, line, (win->_maxx - strlen(buf)) / 2, buf);
    wrefresh(win);
}

int min(int a, int b) {
    return a < b ? a : b;
}

void synchronize_window_with_field(WINDOW* win, char** field, 
                                   int n, int m, int fr, int fc) {
    chtype ch;
    int i, j, row, column;
    for (i = 1; i < win->_maxy; i++) {
        for (j = 1; j < win->_maxx; j++) {
            row = normalize_koordinate(fr + i - 1, n);
            column = normalize_koordinate(fc + j - 1, m);
            if (field[row][column] == 0) {
                ch = ACS_BULLET;
            } else {
                ch = ACS_DIAMOND;
            }
            wmove(win, i, j);
            waddch(win, ch);
        }
    }
    wrefresh(win);
}


void display_field(char** field, int n, int m) {
    WINDOW* display_window;
    int displayn, displaym, ch, row, column, maxn, maxm;
    init_curses();
    maxn = LINES - 8;
    maxm = COLS - 10;
    print_message_center(stdscr, 2, "Use arrow keys to move among field");
    print_message_center(stdscr, 4, "Press ESC to exit");
    displayn = min(maxn, n);
    displaym = min(maxm, m);
    refresh();
    display_window = newwin(displayn + 2, displaym + 2, 
                            (LINES - displayn - 2 - 4) / 2 + 5, 
                            (COLS - displaym - 2) / 2);
    box(display_window, 0, 0);
    wrefresh(display_window);
    synchronize_window_with_field(display_window, field, n, m, 0, 0);
    row = column = 0;
    do {
        ch = getch();
        switch(ch) {
            case KEY_UP:
                row = normalize_koordinate(row - 1, n);
                break;
            case KEY_DOWN:
                row = normalize_koordinate(row + 1, n);
                break;
            case KEY_LEFT:
                column = normalize_koordinate(column - 1, m);
                break;
            case KEY_RIGHT:
                column = normalize_koordinate(column + 1, m);
                break;
        }
        synchronize_window_with_field(display_window, field, n, m, row, column);
    } while (ch != 27);
    clear();
    endwin();
}

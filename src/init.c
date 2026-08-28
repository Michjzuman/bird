#include <ncurses.h>

#define FPS 60

void init_ncurses() {
    initscr();
    noecho();
    curs_set(0);
    cbreak();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    timeout(1000 / FPS);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    printf("\033[?1002h");
    fflush(stdout);
    /*
    start_color();
    init_color(10, 20, 50, 100);
    init_pair(1, COLOR_WHITE, 10);
    bkgd(COLOR_PAIR(1));
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    */
}

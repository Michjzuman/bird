#include <ncurses.h>
#include <stdbool.h>

int main(void) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);

    // Mausbewegungen melden
    printf("\033[?1003h");
    fflush(stdout);

    bool dragging = false;

    int start_x = 0;
    int start_y = 0;

    int ch;

    while ((ch = getch()) != 'q') {
        if (ch != KEY_MOUSE)
            continue;

        MEVENT event;

        if (getmouse(&event) != OK)
            continue;

        if (event.bstate & BUTTON1_PRESSED) {
            dragging = true;

            start_x = event.x;
            start_y = event.y;

            mvprintw(0, 0, "Drag start: %d %d      ",
                start_x, start_y);
        }

        if (dragging) {
            mvprintw(1, 0, "Dragging: %d %d      ",
                event.x, event.y);
        }

        if (event.bstate & BUTTON1_RELEASED) {
            if (dragging) {
                dragging = false;

                mvprintw(2, 0,
                    "Drop: %d %d -> %d %d      ",
                    start_x,
                    start_y,
                    event.x,
                    event.y
                );
            }
        }

        refresh();
    }

    printf("\033[?1003l");
    fflush(stdout);

    endwin();

    return 0;
}
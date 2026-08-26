#include "scene.h"
#include "draw.h"
#include "tick.h"
#include "config.h"
#include "panel_manager.h"

#define FPS 60

Config config;

void init_config() {
    config.fill_void = false;
}

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
    */
}

int main() {
    init_config();

    View view;
    Scene scene;

    init_scene(&scene, &view);

    view.camera = (Camera){0, 0, 0, 0};
    view.mouse.dragging = false;

    init_ncurses();
    update(&scene, &view);
    while (true) {
        draw_scene(&scene, &view);
        update(&scene, &view);
    }
    endwin();
    
    close_scene(&scene);

    return 0;
}



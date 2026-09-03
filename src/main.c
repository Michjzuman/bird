#include "scene.h"
#include "draw.h"
#include "tick.h"
#include "config.h"
#include "editor.h"

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

int main(int argc, char **argv) {
    init_config();

    View view;
    Scene scene;

    init_scene(&scene);

    U16 file_count = 0;
    for (U16 i = 1; i < argc; i++) {
        U16 x = file_count;
        U16 y = 0;
        add_panel_row(&scene);
        add_panel(&scene, x);
        if (!load_editor_file(&scene, x, y, argv[i])) {
            return 1;
        };
        file_count++;
    }
    
    if (file_count > 0) {
        init_view(&view);

        init_ncurses();
        while (true) {
            tick(&scene, &view);
            draw_scene(&scene, &view);
        }
        endwin();
    } else {
        return 1;
    }
    
    close_scene(&scene);

    return 0;
}

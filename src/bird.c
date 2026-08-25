#include "scene.h"
#include "draw.h"

#define FPS 60

void update(Scene *scene, View *view);

struct Mouse mouse = {
    .dragging = false
};

struct Size terminal_size;

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
    getmaxyx(stdscr, terminal_size.h, terminal_size.w);
    /*
    start_color();
    init_color(10, 20, 50, 100);
    init_pair(1, COLOR_WHITE, 10);
    bkgd(COLOR_PAIR(1));
    */
}

int main() {
    View view;
    Scene scene;

    scene.panel_count = 2;
    scene.panels = malloc(scene.panel_count * sizeof(Panel));
    scene.panels[0] = (Panel){
        .type = EDITOR_PANEL,
        .x = 6, .y = 2
    };
    load_editor_file(&scene.panels[0], "./src/bird.c");
    scene.panels[1] = (Panel){
        .type = EDITOR_PANEL,
        .x = 9 + scene.panels[0].w, .y = 2
    };
    load_editor_file(&scene.panels[1], "./src/editor.c");
    /*
    scene.panels[2] = (Panel){
        .type = DUMMY_PANEL,
        .x = 114, .y = 2,
        .w = 50, .h = 20
    };
    */

    view.camera = (Camera){0, 0, 0, 0};

    init_ncurses();
    while (true) {
        draw_scene(&scene, &view);
        update(&scene, &view);
    }
    endwin();
    
    free(scene.panels);

    return 0;
}



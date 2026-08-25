#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "scene.h"

#define FPS 60

struct {
    bool dragging;
    int x, y;
} mouse = {
    .dragging = false
};

struct {U32 w, h;} terminal_size; 

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

void draw_char(Scene *scene, View *view, U32 x, U32 y, char ch) {
    I64 ax = x - view->camera.x;
    I64 ay = y - view->camera.y;
    if (
        ax >= 0 && ax < terminal_size.w &&
        ay >= 0 && ay < terminal_size.h
    ) {
        mvaddch(ay, ax, ch);
    }
}

void draw_content(Scene *scene, View *view, Panel *panel) {
    switch (panel->type) {
        case EDITOR_PANEL:
            draw_editor_content(scene, view, panel);
            break;
        default:
            break;
    }
}

void draw_panels(Scene *scene, View *view) {
    for (U16 i = 0; i < scene->panel_count; i++) {
        Panel p = scene->panels[i];
        for (U8 right = 0; right < 2; right++) {
            for (U8 bottom = 0; bottom < 2; bottom++) {
                draw_char(
                    scene, view,
                    p.x + p.w * right, p.y + p.h * bottom, '+'
                );
                if (bottom == 0) {
                    for (U8 y = 1; y < p.h; y++) {
                        draw_char(
                            scene, view,
                            p.x + p.w * right, p.y + y, '|'
                        );
                    }
                }
                if (right == 0) {
                    for (U8 x = 1; x < p.w; x++) {
                        draw_char(
                            scene, view,
                            p.x + x, p.y + p.h * bottom, '-'
                        );
                    }
                }
            }
        }
    }
}

void draw_scene(Scene *scene, View *view) {
    clear();
    draw_panels(scene, view);
    for (U16 i = 0; i < scene->panel_count; i++) {
        draw_content(scene, view, &scene->panels[i]);
    }
    mvprintw(0, 0, "%f", view->camera.speed_x);
    mvprintw(1, 0, "%f", view->camera.speed_y);
    refresh();
}

void update(Scene *scene, View *view) {
    getmaxyx(stdscr, terminal_size.h, terminal_size.w);

    Camera *camera = &view->camera;

    int key = getch();
    switch (key) {
        case 'q': endwin(); exit(0); break;
        case KEY_UP: camera->speed_y--; break;
        case KEY_RIGHT: camera->speed_x++; break;
        case KEY_DOWN: camera->speed_y++; break;
        case KEY_LEFT: camera->speed_x--; break;
        case KEY_MOUSE: {
            MEVENT event;
            if (getmouse(&event) != OK) break;
            if (event.bstate & BUTTON1_PRESSED) {
                mouse.dragging = true;
                mouse.x = event.x;
                mouse.y = event.y;
            }
            if (event.bstate & BUTTON1_RELEASED) {
                mouse.dragging = false;
            }
            if (mouse.dragging) {
                camera->x += mouse.x - event.x;
                camera->y += mouse.y - event.y;
                mouse.x = event.x;
                mouse.y = event.y;
            }
            break;
        }
    }

    camera->speed_x *= 0.9;
    camera->speed_y *= 0.9;
    if (0.1 < (camera->speed_x > 0 ? camera->speed_x :-camera->speed_x)) {
        camera->x += camera->speed_x;
    }
    if (0.1 < (camera->speed_y > 0 ? camera->speed_y :-camera->speed_y)) {
        camera->y += camera->speed_y;
    }
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

    view.camera = (Camera){0, 0, 0.5, 0};

    init_ncurses();
    while (true) {
        draw_scene(&scene, &view);
        update(&scene, &view);
    }
    endwin();
    
    free(scene.panels);

    return 0;
}



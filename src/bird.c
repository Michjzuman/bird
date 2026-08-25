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

void draw_char(Scene *scene, View *view, U32 x, U32 y, char ch) {
    mvaddch(y - view->camera.y, x - view->camera.x, ch);
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
    draw_panels(scene, view);
    for (U16 i = 0; i < scene->panel_count; i++) {
        draw_content(scene, view, &scene->panels[i]);
    }
}

void update(Scene *scene, View *view) {
    int key = getch();
    switch (key) {
        case 'q': endwin(); exit(0); break;
        case KEY_UP: view->camera.speed_y--; break;
        case KEY_RIGHT: view->camera.speed_x++; break;
        case KEY_DOWN: view->camera.speed_y++; break;
        case KEY_LEFT: view->camera.speed_x--; break;
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
                view->camera.x += mouse.x - event.x;
                view->camera.y += mouse.y - event.y;
                mouse.x = event.x;
                mouse.y = event.y;
            }
            break;
        }
    }

    view->camera.speed_x *= 0.9;
    view->camera.speed_y *= 0.9;
    view->camera.x += view->camera.speed_x;
    view->camera.y += view->camera.speed_y / 2;
}

int main() {
    View view;
    Scene scene;

    scene.panel_count = 3;
    scene.panels = malloc(scene.panel_count * sizeof(Panel));
    scene.panels[0] = (Panel){
        .type = EDITOR_PANEL,
        .x = 6, .y = 2,
        .w = 40, .h = 30,
        .data.editor = load_file("./include/types.h")
    };
    scene.panels[1] = (Panel){
        .type = DUMMY_PANEL,
        .x = 50, .y = 2,
        .w = 60, .h = 50
    };
    scene.panels[2] = (Panel){
        .type = DUMMY_PANEL,
        .x = 114, .y = 2,
        .w = 50, .h = 20
    };

    view.camera = (Camera){0, 0, 0, 0};

    init_ncurses();
    while (true) {
        clear();
        draw_scene(&scene, &view);
        update(&scene, &view);
        refresh();
    }
    endwin();
    
    free(scene.panels);

    return 0;
}



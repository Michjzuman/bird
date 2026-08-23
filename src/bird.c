#include <ncurses.h>
#include <stdlib.h>

#include "scene.h"

void draw_panels(Scene *scene, View *view) {
    for (U16 i = 0; i < scene->panel_count; i++) {
        Panel p = scene->panels[i];
        for (U8 right = 0; right < 2; right++) {
            for (U8 bottom = 0; bottom < 2; bottom++) {
                mvaddch(
                    p.y + p.h * bottom - view->camera.y,
                    p.x + p.w * right - view->camera.x,
                    '+'
                );
                if (bottom == 0) {
                    for (U8 y = 1; y < p.h; y++) {
                        mvaddch(
                            p.y + y - view->camera.y,
                            p.x + p.w * right - view->camera.x,
                            '|'
                        );
                    }
                }
                if (right == 0) {
                    for (U8 x = 1; x < p.w; x++) {
                        mvaddch(
                            p.y + p.h * bottom - view->camera.y,
                            p.x + x - view->camera.x,
                            '-'
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
    refresh();
}

void update(Scene *scene, View *view) {
    int key = getch();
    switch (key) {
        case 'q': endwin(); exit(0); break;
        case KEY_UP: view->camera.speed_y--; break;
        case KEY_RIGHT: view->camera.speed_x++; break;
        case KEY_DOWN: view->camera.speed_y++; break;
        case KEY_LEFT: view->camera.speed_x--; break;
    }

    view->camera.speed_x *= 0.9;
    view->camera.speed_y *= 0.9;
    view->camera.x += view->camera.speed_x;
    view->camera.y += view->camera.speed_y;
}

int main() {
    initscr();
    noecho();
    curs_set(0);
    cbreak();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    timeout(1000 / FPS);

    View view;
    Scene scene;

    scene.panel_count = 3;
    scene.panels = malloc(scene.panel_count * sizeof(Panel));
    scene.panels[0] = (Panel){
        .type = DUMMY_PANEL,
        .x = 6, .y = 7,
        .w = 20, .h = 10
    };
    scene.panels[1] = (Panel){
        .type = DUMMY_PANEL,
        .x = 40, .y = 9,
        .w = 30, .h = 20
    };
    scene.panels[2] = (Panel){
        .type = DUMMY_PANEL,
        .x = 35, .y = 12,
        .w = 30, .h = 20
    };

    view.camera = (Camera){0, 0, 0, 0};

    while (true) {
        draw_scene(&scene, &view);
        update(&scene, &view);
    }

    free(scene.panels);
    endwin();
    return 0;
}



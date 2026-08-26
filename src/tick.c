#include "scene.h"

struct Size terminal_size;
struct Mouse mouse;

void update(Scene *scene, View *view) {
    view->changed = false;

    getmaxyx(stdscr, terminal_size.h, terminal_size.w);

    Camera *camera = &view->camera;

    int key = getch();
    switch (key) {
        case 'q': {endwin(); exit(0);} break;
        case 'w': case KEY_UP: camera->speed_y--; break;
        case 'd': case KEY_RIGHT: camera->speed_x++; break;
        case 's': case KEY_DOWN: camera->speed_y++; break;
        case 'a': case KEY_LEFT: camera->speed_x--; break;
        case KEY_MOUSE: {
            MEVENT event;
            if (getmouse(&event) != OK) break;
            if (event.bstate & BUTTON4_PRESSED) {
                camera->speed_y--;
            }
            if (event.bstate & BUTTON5_PRESSED) {
                camera->speed_y++;
            }
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
    if (0.1 < (camera->speed_x > 0 ? camera->speed_x : -camera->speed_x)) {
        camera->x += camera->speed_x;
        view->changed = true;
    }
    if (0.1 < (camera->speed_y > 0 ? camera->speed_y : -camera->speed_y)) {
        camera->y += camera->speed_y;
        view->changed = true;
    }
}

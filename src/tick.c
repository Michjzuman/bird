#include "scene.h"

void update(Scene *scene, View *view) {
    view->changed = false;

    {
        TerminalSize old = view->terminal_size;
        getmaxyx(stdscr, view->terminal_size.h, view->terminal_size.w);
        if (
            old.w != view->terminal_size.w ||
            old.h != view->terminal_size.h
        ) {
            view->changed = true;
        }
    }

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
                camera->y--;
                view->changed = true;
            }
            /*
            if (event.bstate & BUTTON5_PRESSED) {
                camera->y++;
                view->changed = true;
            }
            */
            if (event.bstate & BUTTON1_PRESSED) {
                view->mouse.dragging = true;
                view->mouse.x = event.x;
                view->mouse.y = event.y;
            }
            if (event.bstate & BUTTON1_RELEASED) {
                view->mouse.dragging = false;
            }
            if (view->mouse.dragging) {
                Camera old_camera = view->camera;
                camera->x += view->mouse.x - event.x;
                camera->y += view->mouse.y - event.y;
                if (old_camera.x != camera->x || old_camera.y != camera->y) {
                    view->changed = true;
                }
                view->mouse.x = event.x;
                view->mouse.y = event.y;
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

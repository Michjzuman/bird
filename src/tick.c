#include "scene.h"
#include "editor.h"

#include <math.h>

#define KEY_ESCAPE 27

void update_content(Scene *scene, View *view, int key) {
    Cursor *c = &view->cursor;
    PanelRow *row = &scene->panel_rows[c->panel_row];
    Panel *panel = &row->panels[c->panel];
    switch (panel->type) {
        case EDITOR_PANEL:
            update_editor_content(scene, view, key);
            break;
        default:
            break;
    }
}

void tick(Scene *scene, View *view) {
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
    Camera old_camera = *camera;

    int key = getch();
    if (view->locked_in) {
        if (key == KEY_ESCAPE) {
            view->locked_in = false;
            view->changed = true;
        } else {
            update_content(scene, view, key);
        }
    } else {
        switch (key) {
            case 'q':
                save_editor_file(
                    &scene->panel_rows[
                        view->cursor.panel_row
                    ].panels[
                        view->cursor.panel
                    ].data.editor
                );
                endwin(); exit(0); break;
            case 'w': case KEY_UP:
                camera->speed_y--; break;
            case 'd': case KEY_RIGHT:
                camera->speed_x++; break;
            case 's': case KEY_DOWN:
                camera->speed_y++; break;
            case 'a': case KEY_LEFT:
                camera->speed_x--; break;
        }
        ///* this is the first part of bird ever written in bird:
        for (U16 x = 0; x < scene->panel_row_count; x++) {
            PanelRow *row = &scene->panel_rows[x];
            for (U16 y = 0; y < row->panel_count; y++) {
                Panel *panel = &row->panels[y];
                if (key == panel->keybind) {
                    camera->x = get_row_x(scene, x);
                    camera->y = get_panel_y(scene, x, y);
                }
            }
        } 
        //*/
    }
    if (key == KEY_MOUSE) {
        MEVENT event;
        if (getmouse(&event) == OK) {
            if (event.bstate & BUTTON4_PRESSED) {
                camera->y--;
            }
            /*
            if (event.bstate & BUTTON5_PRESSED) {
                camera->y++;
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
                camera->x += view->mouse.x - event.x;
                camera->y += view->mouse.y - event.y;
                view->mouse.x = event.x;
                view->mouse.y = event.y;
            }
        }
    }

    camera->speed_x *= 0.9;
    camera->speed_y *= 0.9;
    if (0.1 < fabs(camera->speed_x)) camera->x += camera->speed_x;
    if (0.1 < fabs(camera->speed_y)) camera->y += camera->speed_y;

    if (camera->x != old_camera.x || camera->y != old_camera.y) {
        view->changed = true;
    }
}

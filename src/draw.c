#include "scene.h"
#include "config.h"

#include <string.h>

void draw_char(Scene *scene, View *view, U32 x, U32 y, char ch) {
    I64 cam_x = view->camera.x;
    I64 cam_y = view->camera.y;

    Cursor *c = &view->cursor;
    PanelRow *row = &scene->panel_rows[c->panel_row];
    Panel *panel = &row->panels[c->panel];
    U32 cx = row->x + c->vx + 2 - cam_x;
    U32 cy = panel->y + c->y + 1 - cam_y;
    
    I64 ax = x - cam_x;
    I64 ay = y - cam_y;

    bool is_cursor = ax == cx && ay == cy && view->locked_in;
    if (
        ax >= 0 && ax < view->terminal_size.w &&
        ay >= 0 && ay < view->terminal_size.h
    ) {
        if (is_cursor) {
            //attron(COLOR_PAIR(1));
            mvaddch(ay, ax, ch | A_REVERSE);
            //attroff(COLOR_PAIR(1));
        } else {
            mvaddch(ay, ax, ch);
        }
    }
}

void draw_panel(Scene *scene, View *view, PanelRow *pr, Panel *p) {
    for (U8 right = 0; right < 2; right++) {
        for (U8 bottom = 0; bottom < 2; bottom++) {
            draw_char(
                scene, view,
                pr->x + pr->w * right, p->y + p->h * bottom, '+'
            );
            if (bottom == 0) {
                for (U8 y = 1; y < p->h; y++) {
                    draw_char(
                        scene, view,
                        pr->x + pr->w * right, p->y + y, '|'
                    );
                }
            }
            if (right == 0) {
                for (U8 x = 1; x < pr->w; x++) {
                    draw_char(
                        scene, view,
                        pr->x + x, p->y + p->h * bottom, '-'
                    );
                }
            }
        }
    }
}

void draw_panel_rows(Scene *scene, View *view) {
    for (U16 x = 0; x < scene->panel_row_count; x++) {
        PanelRow *row = &scene->panel_rows[x];
        for (U16 y = 0; y < row->panel_count; y++) {
            draw_panel(scene, view, row, &row->panels[y]);
        }
    }
}

void draw_editor_content(Scene *scene, View *view, PanelRow *pr, Panel *p) {
    EditorData data = p->data.editor;
    for (U32 x = 0; x < strlen(data.path); x++) {
        draw_char(
            scene, view,
            pr->x + x + 2, p->y - 1, data.path[x]
        );
    }
    for (U32 y = 0; y < data.h; y++) {
        for (U32 x = 0; x < data.lines[y].w - 1; x++) {
            char ch = data.lines[y].content[x];
            draw_char(
                scene, view,
                pr->x + x + 2, p->y + y + 1,
                ch == '\n' ? ' ' : ch
            );
        }
        if (config.fill_void) {
            for (U32 x = data.lines[y].w - 2; x < pr->w - 3; x++) {
                draw_char(scene, view, pr->x + x + 2, p->y + y + 1, ':');
            }
        }
    }
}

void draw_content(Scene *scene, View *view, PanelRow *pr, Panel *p) {
    switch (p->type) {
        case EDITOR_PANEL:
            draw_editor_content(scene, view, pr, p);
            break;
        default:
            break;
    }
}

void draw_easteregg(Scene *scene, View *view) {
    draw_char(scene, view, 670, 670, '6');
    draw_char(scene, view, 671, 670, '7');
}

void draw_camera_coors(View *view) {
    if (config.show_camera_coors) {
        mvprintw(0, 0, "x: %f", view->camera.x);
        mvprintw(1, 0, "y: %f", view->camera.y);
    }
}

void draw_scene(Scene *scene, View *view) {
    if (view->changed) {
        clear();
        draw_panel_rows(scene, view);
        for (U16 x = 0; x < scene->panel_row_count; x++) {
            for (U16 y = 0; y < scene->panel_rows[x].panel_count; y++) {
                PanelRow *pr = &scene->panel_rows[x];
                draw_content(scene, view, pr, &pr->panels[y]);
            }
        }
        draw_easteregg(scene, view);
        draw_camera_coors(view);
        /*
        mvprintw(0, 0, "x: %d", view->cursor.x);
        mvprintw(1, 0, "y: %d", view->cursor.y);
        */
        refresh();
    }
}

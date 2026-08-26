#include "scene.h"
#include "config.h"

#include <string.h>

void draw_char(Scene *scene, View *view, U32 x, U32 y, char ch) {
    I64 ax = x - (I64)view->camera.x;
    I64 ay = y - (I64)view->camera.y;
    if (
        ax >= 0 && ax < view->terminal_size.w &&
        ay >= 0 && ay < view->terminal_size.h
    ) {
        mvaddch(ay, ax, ch);
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

void draw_panels(Scene *scene, View *view, PanelRow *panel_row) {
    for (U16 i = 0; i < panel_row->panel_count; i++) {
        draw_panel(scene, view, panel_row, &panel_row->panels[i]);
    }
}

void draw_panel_rows(Scene *scene, View *view) {
    for (U16 i = 0; i < scene->panel_row_count; i++) {
        draw_panels(scene, view, &scene->panel_rows[i]);
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
            if (data.lines[y].content[x] != '\n') {
                draw_char(
                    scene, view,
                    pr->x + x + 2, p->y + y + 1,
                    data.lines[y].content[x]
                );
            }
        }
        if (config.fill_void) {
            for (U32 x = data.lines[y].w - 2; x < pr->w - 3; x++) {
                draw_char(
                    scene, view,
                    pr->x + x + 2, p->y + y + 1,
                    ':'
                );
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
        refresh();
    }
}

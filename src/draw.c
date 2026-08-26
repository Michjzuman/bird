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

void draw_editor_content(Scene *scene, View *view, Panel *panel) {
    EditorData data = panel->data.editor;
    for (U32 x = 0; x < strlen(data.path); x++) {
        draw_char(
            scene, view,
            panel->x + x + 2, panel->y - 1, data.path[x]
        );
    }
    for (U32 y = 0; y < data.h; y++) {
        for (U32 x = 0; x < data.lines[y].w - 1; x++) {
            if (data.lines[y].content[x] != '\n') {
                draw_char(
                    scene, view,
                    panel->x + x + 2, panel->y + y + 1,
                    data.lines[y].content[x]
                );
            }
        }
        if (config.fill_void) {
            for (U32 x = data.lines[y].w - 2; x < panel->w - 3; x++) {
                draw_char(
                    scene, view,
                    panel->x + x + 2, panel->y + y + 1,
                    ':'
                );
            }
        }
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

void draw_scene(Scene *scene, View *view) {
    if (view->changed) {
        clear();
        draw_panels(scene, view);
        for (U16 i = 0; i < scene->panel_count; i++) {
            draw_content(scene, view, &scene->panels[i]);
        }
        refresh();
    }
}

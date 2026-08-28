#include "scene.h"

#include <string.h>

void update_editor_content(Scene *scene, View *view, int key) {
    Cursor *c = &view->cursor;
    PanelRow *row = &scene->panel_rows[c->panel_row];
    Panel *panel = &row->panels[c->panel];
    EditorData *content = &panel->data.editor;
    bool changed = true;
    switch (key) {
        case KEY_UP: {
            if (c->y > 0) {
                c->y--;
                U32 w = content->lines[c->y].w - 2;
                if (c->vx > w) c->vx = w;
            }
            break;
        }
        case KEY_RIGHT: {
            c->x = c->vx + 1;
            break;
        }
        case KEY_DOWN: {
            if (c->y < panel->h - 2) {
                c->y++;
                U32 w = content->lines[c->y].w - 2;
                if (c->vx > w) c->vx = w;
            }
            break;
        }
        case KEY_LEFT: {
            if (c->x > 0) {
                c->x = c->vx - 1;
            }
            break;
        }
        default: {
            if (key >= 32 && key <= 126) {
                EditorLine *line = &content->lines[c->y];
                line->w++;
                if (line->capacity < line->w) {
                    line->capacity *= 2;
                    line->content = realloc(line->content, line->capacity * sizeof(char));
                }
                for (I64 i = line->w - 1; i >= c->vx; i--) {
                    line->content[i] = line->content[i - 1];
                }
                line->content[c->vx] = (char)key;
                c->x++; c->vx++;
                if (line->w + 1 > row->w) row->w = line->w + 1;
            } else {
                changed = false;
            }
            break;
        }
    }
    U32 w = content->lines[c->y].w - 2;
    c->vx = c->x;
    if (c->vx > w) c->vx = w;
    if (changed) view->changed = true;
}

void load_editor_file(PanelRow *panel_row, Panel *panel, char *path) {
    EditorData data;
    data.path = path;

    panel->type = EDITOR_PANEL;
    
    data.capacity = 10;
    data.lines = malloc(data.capacity * sizeof(EditorLine));
    
    FILE *file = fopen(path, "r");

    if (file == NULL) exit(1);

    U32 max_w = 0;
    
    char line[2048];
    U32 y = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (y >= data.capacity) {
            data.capacity *= 2;
            data.lines = realloc(
                data.lines, data.capacity * sizeof(EditorLine)
            );
        }
        EditorLine *nline = &data.lines[y];
        nline->w = strlen(line) + 1;
        if (max_w < nline->w) max_w = nline->w;
        nline->capacity = 1;
        while (nline->capacity < nline->w) {
            nline->capacity *= 2;
        }
        nline->content = malloc(
            nline->capacity * sizeof(char)
        );
        strcpy(nline->content, line);
        y++;
    }
    data.h = y;
    
    fclose(file);
    panel->data.editor = data;
    panel->h = data.h + 1;
    if (panel_row->w < max_w + 1) panel_row->w = max_w + 1;
}

void close_editor_file(EditorData *data) {
    for (U8 y = 0; y < data->h; y++) {
        free(data->lines[y].content);
    }
    free(data->lines);
}

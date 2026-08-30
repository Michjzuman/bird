#include "scene.h"

#include <string.h>

#define LINE_START_CAPACITY 8

void update_editor_panel_size(PanelRow *row, Panel *panel) {
    EditorData *content= &panel->data.editor;
    U32 max_w = 0;
    for (U32 y = 0; y < content->h; y++) {
        EditorLine *line = &content->lines[y];
        if (line->w > max_w) max_w = line->w;
    }
    row->w = max_w + 2;
}

static void init_editor_line(Panel *panel, U32 y) {
    EditorData *content = &panel->data.editor;
    EditorLine *line = &content->lines[y];
    line->capacity = LINE_START_CAPACITY;
    line->w = 1;
    line->content = malloc(line->capacity * sizeof(char));
    line->content[0] = '\n';
}

static void wrap_editor_line(Panel *panel, U32 y1, U32 x1, U32 y2) {
    EditorData *content = &panel->data.editor;
    EditorLine *line1 = &content->lines[y1];
    EditorLine *line2 = &content->lines[y2];
    {
        line2->w = line1->w - x1;
        line2->capacity = LINE_START_CAPACITY;
        while (line2->capacity < line2->w) line2->capacity *= 2;
        line2->content = malloc(line2->capacity * sizeof(char));
        for (U32 i = 0; i < line2->w; i++) {
            line2->content[i] = line1->content[i + x1];
        }
    }
    {
        line1->w = x1 + 1;
        while (line1->capacity > line1->w * 2) line1->capacity /= 2;
        line1->content = realloc(line1->content, line1->capacity * sizeof(char));
        line1->content[line1->w - 1] = '\n';
    }
}

static void add_editor_line(Panel *panel, U32 y) {
    EditorData *content = &panel->data.editor;
    content->h++;
    if (content->capacity < content->h) {
        content->capacity += 10;
        content->lines = realloc(content->lines, content->capacity * sizeof(EditorLine));
    }
    for (I64 i = content->h - 1; i > y + 1; i--) {
        content->lines[i] = content->lines[i - 1];
    }
    panel->h++;
}

void update_editor_content(Scene *scene, View *view, int key) {
    Cursor *c = &view->cursor;
    PanelRow *row = &scene->panel_rows[c->panel_row];
    Panel *panel = &row->panels[c->panel];
    EditorData *content = &panel->data.editor;
    EditorLine *line = &content->lines[c->y];
    bool changed = true;
    switch (key) {
        case KEY_ENTER: case '\n': {
            add_editor_line(panel, c->y);
            wrap_editor_line(panel, c->y, c->vx, c->y + 1);
            update_editor_panel_size(row, panel);
            c->y++; c->x = 0;
            break;
        }
        case KEY_DOWN: {
            if (c->y < panel->h - 2) {
                c->y++;
                U32 w = content->lines[c->y].w - 1;
                if (c->vx > w) c->vx = w;
            }
            break;
        }
        case KEY_UP: {
            if (c->y > 0) {
                c->y--;
                U32 w = content->lines[c->y].w - 1;
                if (c->vx > w) c->vx = w;
            }
            break;
        }
        case KEY_RIGHT: {
            if (line->w > 1) c->x = c->vx + 1;
            break;
        }
        case KEY_BACKSPACE: case 127: case '\b': {
            if (c->x > 0 && line->w > 1) {
                for (U32 i = c->vx - 1; i < line->w - 1; i++) {
                    line->content[i] = line->content[i + 1];
                }
                line->w--;
                if (line->w < line->capacity / 2) {
                    line->capacity /= 2;
                    line->content = realloc(line->content, line->capacity * sizeof(char));
                }
                update_editor_panel_size(row, panel);
                // fall through
            } else break;
        }
        case KEY_LEFT: {
            if (c->vx > 0) {
                c->x = c->vx - 1;
            }
            break;
        }
        default: {
            if (key >= 32 && key <= 126) {
                line->w++;
                if (line->capacity < line->w) {
                    line->capacity *= 2;
                    line->content = realloc(line->content, line->capacity * sizeof(char));
                }
                for (I64 i = line->w - 1; i >= c->vx; i--) {
                    line->content[i] = line->content[i - 1];
                }
                line->content[c->vx] = (char)key;
                c->x = c->vx + 1;
                if (line->w + 2 > row->w) row->w = line->w + 2;
            } else {
                changed = false;
            }
            break;
        }
    }
    U32 w = content->lines[c->y].w - 1;
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
        nline->w = strlen(line);
        if (max_w < nline->w) max_w = nline->w;
        nline->capacity = LINE_START_CAPACITY;
        while (nline->capacity < nline->w) {
            nline->capacity *= 2;
        }
        nline->content = malloc(
            nline->capacity * sizeof(char)
        );
        for (U32 i = 0; i < nline->w; i++) nline->content[i] = line[i];
        y++;
    }
    data.h = y;
    
    fclose(file);
    panel->data.editor = data;
    panel->h = data.h + 1;
    if (panel_row->w < max_w + 1) panel_row->w = max_w + 2;
}

void close_editor_file(EditorData *data) {
    for (U8 y = 0; y < data->h; y++) {
        free(data->lines[y].content);
    }
    free(data->lines);
}

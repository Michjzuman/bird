#include "scene.h"

#include <string.h>

#define LINE_START_CAPACITY 8
#define CONTENT_CAPACITY_BUFFER 10

void adjust_line_capacity(EditorLine *line) {
    U32 old_capacity = line->capacity;
    if (line->capacity < line->w) {
        while (line->capacity < line->w) line->capacity *= 2;
    } else {
        while (line->capacity >= line->w * 2) line->capacity /= 2;
    }
    if (old_capacity != line->capacity) {
        line->content = realloc(line->content, line->capacity * sizeof(char));
    }
}

void update_editor_panel_size(PanelRow *row) {
    U32 max_w = 0;
    for (U32 i = 0; i < row->panel_count; i++) {
        EditorData *content= &row->panels[i].data.editor;
        for (U32 y = 0; y < content->h; y++) {
            EditorLine *line = &content->lines[y];
            if (line->w > max_w) max_w = line->w;
        }
    }
    row->w = max_w + 2;
}

static void wrap_editor_line(Panel *panel, U32 y1, U32 x1, U32 y2, bool care_about_line1) {
    EditorData *content = &panel->data.editor;
    EditorLine *line1 = &content->lines[y1];
    EditorLine *line2 = &content->lines[y2];
    {
        U32 old_line2_w = line2->w;
        line2->w += line1->w - x1 - 1;
        adjust_line_capacity(line2);
        for (U32 i = old_line2_w - 1; i < line2->w; i++) {
            line2->content[i] = line1->content[i + x1 - (old_line2_w - 1)];
        }
    }
    if (care_about_line1) {
        line1->w = x1 + 1;
        adjust_line_capacity(line1);
        line1->content[line1->w - 1] = '\n';
    }
}

static void add_editor_line(Panel *panel, U32 y) {
    EditorData *content = &panel->data.editor;
    content->h++;
    if (content->capacity < content->h) {
        content->capacity += CONTENT_CAPACITY_BUFFER;
        content->lines = realloc(content->lines, content->capacity * sizeof(EditorLine));
    }
    for (I64 i = content->h - 1; i > y + 1; i--) {
        content->lines[i] = content->lines[i - 1];
    }
    EditorLine *line = &content->lines[y + 1];
    line->w = 1;
    line->content = malloc(0);
    line->capacity = LINE_START_CAPACITY;
    panel->h++;
}

static void delete_editor_line(Panel *panel, U32 y) {
    EditorData *content = &panel->data.editor;
    EditorLine *line = &content->lines[y];
    free(line->content);
    for (I64 i = y; i < panel->h - 2; i++) {
        content->lines[i] = content->lines[i + 1];
    }
    if (content->capacity > content->h + CONTENT_CAPACITY_BUFFER) {
        content->capacity -= CONTENT_CAPACITY_BUFFER;
        content->lines = realloc(content->lines, content->capacity * sizeof(EditorLine));
    }
    content->h--;
    panel->h--;
}

void update_editor_content(Scene *scene, View *view, int key) {
    Cursor *c = &view->cursor;
    PanelRow *row = &scene->panel_rows[c->panel_row];
    Panel *panel = &row->panels[c->panel];
    EditorData *content = &panel->data.editor;
    EditorLine *line = &content->lines[c->y];
    bool changed = true;
    /*
    for (U32 i = 0; i < content->h; i++) {
        mvprintw(20 + i, 0, "%d", content->lines[i].w);
        mvprintw(20 + i, 4, "%d", content->lines[i].capacity);
        mvprintw(20 + i, 8, "%p", content->lines[i].content);
    }
    */
    switch (key) {
        case KEY_ENTER: case '\n': {
            add_editor_line(panel, c->y);
            wrap_editor_line(panel, c->y, c->vx, c->y + 1, true);
            update_editor_panel_size(row);
            c->y++; c->x = 0;
            break;
        }
        case KEY_DOWN: {
            if (c->y < content->h - 1) {
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
            c->x = c->vx + 1;
            U32 w = content->lines[c->y].w - 1;
            if (c->x > w) c->x = w;
            break;
        }
        case KEY_BACKSPACE: case 127: case '\b': {
            if (c->x > 0 && line->w > 1) {
                for (U32 i = c->vx - 1; i < line->w - 1; i++) {
                    line->content[i] = line->content[i + 1];
                }
                line->w--;
                adjust_line_capacity(line);
                update_editor_panel_size(row);
                // fall through
            } else if (c->y > 0) {
                U32 cx = content->lines[c->y - 1].w - 1;
                wrap_editor_line(panel, c->y, 0, c->y - 1, false);
                delete_editor_line(panel, c->y);
                U32 w = content->lines[c->y - 1].w;
                if (w > row->w) row->w = w + 2;
                c->y--; c->x = cx;
                break;
            } else break;;
        }
        case KEY_LEFT: {
            if (c->vx > 0) c->x = c->vx - 1;
            else c->x = 0;
            break;
        }
        default: {
            if (key >= 32 && key <= 126) {
                line->w++;
                adjust_line_capacity(line);
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

bool load_editor_file(Scene *scene, U16 x, U16 y, char *path) {
    PanelRow *row = &scene->panel_rows[x];
    Panel *panel = &row->panels[y];
    EditorData data;
    data.path = path;

    panel->type = EDITOR_PANEL;
    
    data.capacity = CONTENT_CAPACITY_BUFFER;
    data.lines = malloc(data.capacity * sizeof(EditorLine));
    
    FILE *file = fopen(path, "r");
    if (file == NULL) return false;

    U32 max_w = 0;
    for (U16 i = 0; i < row->panel_count; i++) {
        if (row->w > max_w + 2) max_w = row->w - 2;
    }
    
    char line[2048];
    U32 h = 0;
    bool done = false;
    char *check;
    while ((check = fgets(line, sizeof(line), file)) != NULL || !done) {
        bool empty_line = check == NULL;
        bool last = empty_line || line[strlen(line) - 1] != '\n';
        if (last) done = true;
        if (h >= data.capacity) {
            data.capacity *= 2;
            data.lines = realloc(
                data.lines, data.capacity * sizeof(EditorLine)
            );
        }
        EditorLine *nline = &data.lines[h];
        nline->w = (empty_line ? 0 : strlen(line)) + last;
        if (max_w < nline->w) max_w = nline->w;
        nline->capacity = LINE_START_CAPACITY;
        while (nline->capacity < nline->w) nline->capacity *= 2;
        nline->content = malloc(nline->capacity * sizeof(char));
        for (U32 i = 0; i < nline->w - last; i++) nline->content[i] = line[i];
        if (last) nline->content[nline->w - 1] = '\0';
        h++;
    }
    data.h = h;
    
    fclose(file);
    panel->data.editor = data;
    panel->h = data.h + 1;
    if (row->w < max_w + 2) row->w = max_w + 2;
    return true;
}

void close_editor_file(EditorData *data) {
    for (U32 y = 0; y < data->h; y++) {
        free(data->lines[y].content);
    }
    free(data->lines);
}

bool save_editor_file(EditorData *data) {
    FILE *file = fopen(data->path, "w");
    if (file == NULL) return false;
    for (U32 i = 0; i < data->h; i++) {
        EditorLine *line = &data->lines[i];
        char *text = malloc((line->w + 1) * sizeof(char));
        memcpy(text, line->content, line->w * sizeof(char));
        text[line->w] = '\0';
        fprintf(file, "%s", text);
        free(text);
    }
    fclose(file);
    return true;
}

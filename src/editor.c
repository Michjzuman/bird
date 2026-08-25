#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "scene.h"

void load_editor_file(Panel *panel, char *path) {
    EditorData data;
    data.path = path;

    data.capacity = 10;
    data.lines = malloc(data.capacity * sizeof(EditorLine));
    
    FILE *file = fopen(path, "r");

    if (file == NULL) exit(0);

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
    panel->w = max_w + 1;
}

void close_editor_file(EditorData *data) {
    for (U8 y = 0; y < data->h; y++) {
        free(data->lines[y].content);
    }
    free(data->lines);
}

void draw_editor_content(Scene *scene, View *view, Panel *panel) {
    EditorData data = panel->data.editor;
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
    }
}


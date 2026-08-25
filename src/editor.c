#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "scene.h"

EditorData load_file(char *path) {
    EditorData data;
    data.path = path;

    data.capacity = 10;
    data.lines = malloc(data.capacity * sizeof(EditorLine));
    
    FILE *file = fopen(path, "r");

    if (file == NULL) exit(0);
    
    char line[2048];
    U32 y = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (y >= data.capacity) {
            data.capacity *= 2;
            data.lines = realloc(
                data.lines, data.capacity * sizeof(EditorLine)
            );
        }
        data.lines[y].w = strlen(line) + 1;
        data.lines[y].capacity = 1;
        while (data.lines[y].capacity < data.lines[y].w) {
            data.lines[y].capacity *= 2;
        }
        data.lines[y].content = malloc(
            data.lines[y].capacity * sizeof(char)
        );
        strcpy(data.lines[y].content, line);
        y++;
    }
    data.h = y;
    
    fclose(file);
    return data;
}

void close_file(EditorData *data) {
    for (U8 y = 0; y < data->h; y++) {
        free(data->lines[y].content);
    }
    free(data->lines);
}

void draw_editor_content(Scene *scene, View *view, Panel *panel) {
    EditorData data = panel->data.editor;
    for (U32 y = 0; y < data.h; y++) {
        for (U32 x = 0; x < data.lines[y].w - 1; x++) {
            draw_char(
                scene, view,
                panel->x + x + 2, panel->y + y + 1,
                data.lines[y].content[x]
            );
        }
    }
}


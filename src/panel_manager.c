#include "scene.h"


void init_panel(PanelRow *panel_row, Panel *panel) {
    panel->type = EDITOR_PANEL;
    load_editor_file(panel_row, panel, "src/draw.c");
}

void init_panel_row(PanelRow *panel_row) {
    panel_row->panel_count = 3;
    panel_row->panels = malloc(panel_row->panel_count * sizeof(Panel));
    U32 y = 1;
    for (U16 i = 0; i < panel_row->panel_count; i++) {
        Panel *panel = &panel_row->panels[i];
        init_panel(panel_row, panel);
        panel->y = y;
        y += panel->h + 3;
    }
}

void init_scene(Scene *scene, View *view) {
    scene->panel_row_count = 10;
    scene->panel_rows = malloc(scene->panel_row_count * sizeof(PanelRow));
    U32 x = 0;
    for (U16 i = 0; i < scene->panel_row_count; i++) {
        PanelRow *panel_row = &scene->panel_rows[i];
        init_panel_row(panel_row);
        panel_row->x = x;
        x += panel_row->w + 4;
    }
}

void close_scene(Scene *scene) {
    free(scene->panel_rows);
}
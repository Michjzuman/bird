#include "scene.h"
#include "config.h"

void init_panel_row(PanelRow *row) {
    row->panel_count = 0;
    row->panels = malloc(0);
}

void init_scene(Scene *scene) {
    scene->panel_row_count = 0;
    scene->panel_rows = malloc(0);
}

void close_scene(Scene *scene) {
    for (U16 i = 0; i < scene->panel_row_count; i++) {
        free(scene->panel_rows[i].panels);
    }
    free(scene->panel_rows);
}

PanelRow *add_panel_row(Scene *scene) {
    U16 x = 0;
    for (U16 i = 0; i < scene->panel_row_count; i++) {
        x += scene->panel_rows[i].w + config.margin_x;
    }
    scene->panel_row_count++;
    scene->panel_rows = realloc(scene->panel_rows, scene->panel_row_count * sizeof(PanelRow));
    PanelRow *row = &scene->panel_rows[scene->panel_row_count - 1];
    init_panel_row(row);
    row->x = x;
    return row;
}

Panel *add_panel(PanelRow *row) {
    U16 y = 1;
    for (U16 i = 0; i < row->panel_count; i++) {
        y += row->panels[i].h + config.margin_y;
    }
    row->panel_count++;
    row->panels = realloc(row->panels, row->panel_count * sizeof(Panel));
    Panel *panel = &row->panels[row->panel_count - 1];
    panel->y = y;
    return panel;
}

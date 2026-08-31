#include "scene.h"
#include "config.h"

U32 get_row_x(Scene *scene, PanelRow *row) {
    U32 result = 0;
    for (U16 i = 0; i < scene->panel_row_count; i++) {
        if (row == &scene->panel_rows[i]) {
            return result;
        }
        result += scene->panel_rows[i].w + config.margin_x;
    }
    return 0;
}

U32 get_panel_y(PanelRow *row, Panel *panel) {
    U32 result = 1;
    for (U16 i = 0; i < row->panel_count; i++) {
        if (panel == &row->panels[i]) {
            return result;
        }
        result += row->panels[i].h + config.margin_y;
    }
    return 0;
}

static void init_panel_row(PanelRow *row) {
    row->panel_count = 0;
    row->panels = malloc(0);
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
    return panel;
}

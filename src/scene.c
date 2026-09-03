#include "scene.h"
#include "config.h"

void init_scene(Scene *scene) {
    scene->panel_row_count = 0;
    scene->panel_rows = malloc(0);
}

U32 get_row_x(Scene *scene, U16 x) {
    U32 result = 0;
    for (U16 i = 0; i < scene->panel_row_count; i++) {
        if (i >= x) break;
        result += scene->panel_rows[i].w + config.margin_x;
    }
    return result;
}

U32 get_panel_y(Scene *scene, U16 x, U16 y) {
    PanelRow *row = &scene->panel_rows[x];
    U32 result = 1;
    for (U16 i = 0; i < row->panel_count; i++) {
        if (i >= y) break;
        result += row->panels[i].h + config.margin_y;
    }
    return result;
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

void add_panel_row(Scene *scene) {
    U16 x = 0;
    for (U16 i = 0; i < scene->panel_row_count; i++) {
        x += scene->panel_rows[i].w + config.margin_x;
    }
    scene->panel_row_count++;
    scene->panel_rows = realloc(
        scene->panel_rows, scene->panel_row_count * sizeof(PanelRow)
    );
    PanelRow *row = &scene->panel_rows[scene->panel_row_count - 1];
    init_panel_row(row);
}

void add_panel(Scene *scene, U16 x) {
    static int keybinds[] = {
        '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '\''
    };
    static U8 next_keybind = 0;
    PanelRow *row = &scene->panel_rows[x];
    U16 y = 1;
    for (U16 i = 0; i < row->panel_count; i++) {
        y += row->panels[i].h + config.margin_y;
    }
    row->panel_count++;
    row->panels = realloc(row->panels, row->panel_count * sizeof(Panel));
    Panel *panel = &row->panels[row->panel_count - 1];
    panel->keybind = keybinds[next_keybind];
    next_keybind++;
}

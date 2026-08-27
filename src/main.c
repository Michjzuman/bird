#include "scene.h"
#include "draw.h"
#include "tick.h"
#include "config.h"
#include "panel_manager.h"

void init_ncurses(void);

int main() {
    init_config();

    View view;
    Scene scene;

    init_scene(&scene);

    {
        PanelRow *row = add_panel_row(&scene);
        {
            Panel *panel = add_panel(row);
            load_editor_file(row, panel, "include/types.h");
        }
        {
            Panel *panel = add_panel(row);
            load_editor_file(row, panel, "include/scene.h");
        }
    }
    {
        PanelRow *row = add_panel_row(&scene);
        {
            Panel *panel = add_panel(row);
            load_editor_file(row, panel, "src/main.c");
        }
    }
    {
        PanelRow *row = add_panel_row(&scene);
        {
            Panel *panel = add_panel(row);
            load_editor_file(row, panel, "include/types.h");
        }
        {
            Panel *panel = add_panel(row);
            load_editor_file(row, panel, "include/scene.h");
        }
    }

    view.camera = (Camera){0, 0, 0, 0};
    view.mouse.dragging = false;

    init_ncurses();
    update(&scene, &view);
    while (true) {
        draw_scene(&scene, &view);
        update(&scene, &view);
    }
    endwin();
    
    close_scene(&scene);

    return 0;
}



#include "scene.h"
#include "draw.h"
#include "tick.h"
#include "config.h"
#include "panel_manager.h"
#include "editor.h"
#include "init.h"

int main() {
    init_config();

    View view;
    Scene scene;

    init_scene(&scene);

    {
        PanelRow *row = add_panel_row(&scene);
        {
            Panel *panel = add_panel(row);
            load_editor_file(row, panel, "test.txt");
            //add_panel_keybind(&scene, row, panel, '1');
        }
        {
            Panel *panel = add_panel(row);
            load_editor_file(row, panel, "include/panel.h");
            //add_panel_keybind(&scene, row, panel, '2');
        }
    }
    {
        PanelRow *row = add_panel_row(&scene);
        Panel *panel = add_panel(row);
        load_editor_file(row, panel, "src/main.c");
        //add_panel_keybind(&scene, row, panel, '3');
    }

    init_view(&view);

    init_ncurses();
    while (true) {
        tick(&scene, &view);
        draw_scene(&scene, &view);
    }
    endwin();
    
    close_scene(&scene);

    return 0;
}

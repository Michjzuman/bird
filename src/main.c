#include "scene.h"
#include "draw.h"
#include "tick.h"
#include "config.h"
#include "panel_manager.h"
#include "editor.h"

void init_ncurses(void);

int main() {
    init_config();

    View view;
    Scene scene;

    init_scene(&scene);

    {
        PanelRow *row = add_panel_row(&scene);
        Panel *panel = add_panel(row);
        load_editor_file(row, panel, "test.txt");
    }

    view.camera = (Camera){0, 0, 0, 0};
    view.mouse.dragging = false;
    view.cursor = (Cursor){0, 0, 0, 0, 0};
    view.locked_in = true;

    init_ncurses();
    while (true) {
        tick(&scene, &view);
        draw_scene(&scene, &view);
    }
    endwin();
    
    close_scene(&scene);

    return 0;
}

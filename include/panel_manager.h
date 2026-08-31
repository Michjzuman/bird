#ifndef PANEL_MANAGER_H
#define PANEL_MANAGER_H

#include "scene.h"

void close_scene(Scene *);

PanelRow *add_panel_row(Scene *);

Panel *add_panel(PanelRow *);

U32 get_row_x(Scene *, PanelRow *);

U32 get_panel_y(PanelRow *, Panel *);

void add_panel_keybind(Scene *, PanelRow *, Panel *, int);

#endif

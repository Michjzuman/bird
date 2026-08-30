#ifndef PANEL_MANAGER_H
#define PANEL_MANAGER_H

#include "scene.h"

void close_scene(Scene *);

PanelRow *add_panel_row(Scene *);

Panel *add_panel(PanelRow *);

#endif

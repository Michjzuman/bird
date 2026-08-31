#ifndef SCENE_H
#define SCENE_H

#include <curses.h>

#include "types.h"
#include "view.h"
#include "panel.h"

typedef struct {
    int key;
    PanelRow *panel_row;
    Panel *panel;
} PanelKeybind;

typedef struct {
    U16 panel_row_count;
    PanelRow *panel_rows;
    U8 sticky_panel_count;
    StickyPanel *sticky_panels;
    U8 panel_keybind_count;
    PanelKeybind *panel_keybinds;
} Scene;

#endif

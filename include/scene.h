#ifndef SCENE_H
#define SCENE_H

#include <ncurses.h>

#include "types.h"
#include "view.h"
#include "panel.h"

typedef struct {
    U16 panel_count;
    Panel *panels;
    U8 sticky_panel_count;
    StickyPanel *sticky_panels;
} Scene;

extern struct Size {
    U32 w, h;
} terminal_size;

extern struct Mouse {
    bool dragging;
    int x, y;
} mouse;

#endif
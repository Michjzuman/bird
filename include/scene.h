#ifndef SCENE_H
#define SCENE_H

#include <curses.h>

#include "types.h"
#include "view.h"

#define INCLUDING_FROM_SCENE_H
#include "editor.h"
#undef INCLUDING_FROM_SCENE_H

enum PanelType {
    DUMMY_PANEL,
    EDITOR_PANEL,
    TERMINAL_PANEL
};

typedef union {
    EditorData editor;
} PanelData;

typedef struct {
    U8 type;
    U32 h;
    PanelData data;
    int keybind;
} Panel;

typedef struct {
    U32 w;
    U16 panel_count;
    Panel *panels;
} PanelRow;

typedef struct {
    U32 x, y, w, h;
    U8 side;
} StickyPanel;

typedef struct {
    U16 panel_row_count;
    PanelRow *panel_rows;
    U8 sticky_panel_count;
    StickyPanel *sticky_panels;
} Scene;

void init_scene(Scene *);

void close_scene(Scene *);

void add_panel_row(Scene *);

void add_panel(Scene *, U16 x);

U32 get_row_x(Scene *, U16 x);

U32 get_panel_y(Scene *, U16 x, U16 y);

#endif

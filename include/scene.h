#ifndef BIRD_H
#define BIRD_H

#include "types.h"

typedef struct {
    double x, y;
    double speed_x, speed_y;
} Camera;

typedef struct {
    Camera camera;
    U16 cursor_panel;
    U32 x, y;
} View;

enum PanelType {
    DUMMY_PANEL,
    EDITOR_PANEL,
    TERMINAL_PANEL
};

typedef struct {
    char *path;
    char *content;
} EditorData;

typedef union {
    EditorData editor;
} PanelData;

typedef struct {
    U32 x, y, w, h;
    U8 type;
    PanelData data;
} Panel;

typedef struct {
    U32 x, y, w, h;
    U8 side;
} StickyPanel;

typedef struct {
    U16 panel_count;
    Panel *panels;
    U8 sticky_panel_count;
    StickyPanel *sticky_panels;
} Scene;

#endif
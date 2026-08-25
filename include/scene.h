#ifndef BIRD_H
#define BIRD_H

#include "types.h"
#include "editor.h"

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

void draw_editor_content(Scene *scene, View *view, Panel *panel);
void load_editor_file(Panel *panel, char *path);
void close_editor_file(EditorData *data);

void draw_char(Scene *scene, View *view, U32 x, U32 y, char ch);

#endif
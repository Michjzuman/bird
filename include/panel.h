#ifndef PANEL_H
#define PANEL_H

#include "types.h"

typedef struct {
    U32 w;
    char *content;
    U32 capacity;
} EditorLine;

typedef struct {
    char *path;
    U32 h;
    EditorLine *lines;
    U32 capacity;
} EditorData;

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

#endif

#ifndef VIEW_H
#define VIEW_H

#include "types.h"

typedef struct {
    double x, y;
    double speed_x, speed_y;
} Camera;

typedef struct {
    U16 panel_row, panel;
    U32 x, vx, y;
} Cursor;

typedef struct {
    U32 w, h;
} TerminalSize;

typedef struct {
    bool dragging;
    int x, y;
} Mouse;

typedef struct {
    bool changed;
    Camera camera;
    Cursor cursor;
    Mouse mouse;
    TerminalSize terminal_size;
    bool locked_in;
} View;

void init_view(View *view);

#endif

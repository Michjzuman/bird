#ifndef VIEW_H
#define VIEW_H

#include "types.h"

typedef struct {
    double x, y;
    double speed_x, speed_y;
} Camera;

typedef struct {
    U16 panel;
    U32 x, y;
} Cursor;

struct Size {
    U32 w, h;
};

struct Mouse {
    bool dragging;
    int x, y;
};

typedef struct {
    Camera camera;
    Cursor cursor;
    bool changed;
    struct Mouse mouse;
    struct Size terminal_size;
} View;

#endif

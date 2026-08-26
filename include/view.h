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

typedef struct {
    Camera camera;
    Cursor cursor;
    bool changed;
} View;

#endif

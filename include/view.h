#ifndef VIEW_H
#define VIEW_H

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

#endif
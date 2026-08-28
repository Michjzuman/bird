#ifndef CONFIG_H
#define CONFIG_H

#include "types.h"

typedef struct {
    char *top_right;
    char *top_left;
    char *bottom_right;
    char *bottom_left;
    char *horizontal;
    char *vertical;
} Theme;

typedef struct {
    Theme theme;
    bool fill_void;
    U8 margin_x, margin_y;
    bool show_camera_coors;
} Config;

extern Config config;

void init_config(void);

#endif

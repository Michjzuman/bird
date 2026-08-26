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
} Config;

extern Config config;

#endif

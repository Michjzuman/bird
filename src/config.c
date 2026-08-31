#include "config.h"

Config config;

void init_config() {
    config.fill_void = false;
    config.margin_x = 3;
    config.margin_y = 3;
    config.show_camera_coors = false;
    config.newline_dot = true;
}

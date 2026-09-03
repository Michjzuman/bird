#ifndef DRAW_H
#define DRAW_H

#include "scene.h"

void draw_char(Scene *scene, View *view, U32 x, U32 y, char ch);

void draw_scene(Scene *scene, View *view);

#endif
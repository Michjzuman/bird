#ifndef DRAW_H
#define DRAW_H

#include "scene.h"

void draw_char(Scene *scene, View *view, U32 x, U32 y, char ch);

void draw_content(Scene *scene, View *view, Panel *panel);

void draw_panels(Scene *scene, View *view);

void draw_scene(Scene *scene, View *view);

void draw_editor_content(Scene *scene, View *view, Panel *panel);

#endif
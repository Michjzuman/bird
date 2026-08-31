#ifndef EDITOR_H
#define EDITOR_H

#include "scene.h"

void update_editor_content(Scene *, View *, int key);

bool load_editor_file(PanelRow *panel_row, Panel *panel, char *path);
void close_editor_file(EditorData *data);
bool save_editor_file(EditorData *data);

#endif

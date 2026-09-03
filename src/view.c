#include "view.h"

void init_view(View *view) {
    view->mouse.dragging = false;
    view->cursor = (Cursor){0, 0, 0, 0, 0};
    view->locked_in = true;
    view->camera = (Camera){-10.0f, -5.0f, 0, 0};
}
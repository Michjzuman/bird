#ifndef EDITOR_H
#define EDITOR_H

enum EditorLanguage {
    UNKNOWN_LANG, C_LANG,
    PYTHON_LANG, MARKDOWN_LANG
};

typedef struct {
    U32 w;
    char *content;
    U32 capacity;
} EditorLine;

typedef struct {
    char *path;
    U32 h;
    EditorLine *lines;
    U32 capacity;
} EditorData;

#endif

#ifndef INCLUDING_FROM_SCENE_H
    #ifndef EDITOR_FUNCTIONS_H
    #define EDITOR_FUNCTIONS_H
    #include "scene.h"
    void update_editor_content(Scene *, View *, int key);
    bool load_editor_file(Scene *scene, U16 x, U16 y, char *path);
    void close_editor_file(EditorData *data);
    bool save_editor_file(EditorData *data);
    #endif
#endif

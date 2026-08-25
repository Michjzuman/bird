#ifndef EDITOR_H
#define EDITOR_H

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
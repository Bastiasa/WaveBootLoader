#ifndef WAVE_GRAPHIC_OBJECT_IMPL
#define WAVE_GRAPHIC_OBJECT_IMPL

#include "vector2.h"
#include "canvas.h"

typedef struct 
{
    void *Parent;
    WAVE_CANVAS_MANAGER *Canvas;
    CHAR16 *Name;
    uint8_t Visible;

    VEC2 Position;
    VEC2 Scale;

    VEC2 FinalPosition;
    VEC2 FinalScale;


    void **Children;
    void (* Render)(void *object);
} WAVE_GRAPHIC_OBJECT;


void HideObject(WAVE_GRAPHIC_OBJECT *obj);
void ShowObject(WAVE_GRAPHIC_OBJECT *obj);

void ObjectAddChild(WAVE_GRAPHIC_OBJECT *parent, void *child);
uint8_t ObjectRemoveChild(WAVE_GRAPHIC_OBJECT *parent, void *child);

void OnObjectRender(WAVE_GRAPHIC_OBJECT *obj);


WAVE_GRAPHIC_OBJECT CreateEmptyObject(WAVE_CANVAS_MANAGER *canvas, VEC2 position, void (*render)(void *child));

#endif
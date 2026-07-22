#include "vector2.h"
#include "canvas.h"

typedef struct 
{
    WAVE_CANVAS_MANAGER *Canvas;
    uint8_t Visible;
    VEC2 Position;
    VEC2 Scale;
    void (* Render)(void *object);
} WAVE_GRAPHIC_OBJECT;


void HideObject(WAVE_GRAPHIC_OBJECT *obj);
void ShowObject(WAVE_GRAPHIC_OBJECT *obj);

WAVE_GRAPHIC_OBJECT CreateEmptyObject(WAVE_CANVAS_MANAGER *canvas, VEC2 position, void (*render)(void *child));
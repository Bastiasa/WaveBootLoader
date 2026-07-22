#include "object.h"

void HideObject(WAVE_GRAPHIC_OBJECT *obj)
{
    obj->Visible = 0;
}

void ShowObject(WAVE_GRAPHIC_OBJECT *obj)
{
    obj->Visible = 1;
}

WAVE_GRAPHIC_OBJECT CreateEmptyObject(WAVE_CANVAS_MANAGER *canvas, VEC2 position, void (*render)(void *child))
{
    return (WAVE_GRAPHIC_OBJECT) {
        .Render = render,
        .Canvas = canvas,
        .Position = position,
        .Scale = VECTOR2_ONE,
        .Visible = 1
    };
}

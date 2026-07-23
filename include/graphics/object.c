#include <stb_ds.h>

#include "object.h"

void HideObject(WAVE_GRAPHIC_OBJECT *obj)
{
    obj->Visible = 0;
}

void ShowObject(WAVE_GRAPHIC_OBJECT *obj)
{
    obj->Visible = 1;
}

void OnObjectRender(WAVE_GRAPHIC_OBJECT *obj) {
    
    if (obj->Parent == NULL)
    {
        obj->FinalPosition = obj->Position;
        obj->FinalScale = obj->Scale;
        return;
    }

    WAVE_GRAPHIC_OBJECT *CastedParent = (WAVE_GRAPHIC_OBJECT *)obj->Parent;

    obj->FinalPosition = VectorSum(CastedParent->FinalPosition, VectorMultiply(obj->Position, CastedParent->FinalScale));
    obj->FinalScale = VectorMultiply(CastedParent->FinalScale, obj->Scale);
}

uint8_t ObjectRemoveChild(WAVE_GRAPHIC_OBJECT *parent, void *child) {
    if (parent == child)
    {
        return;
    }
    
    for (size_t i = 0; i < arrlen(parent->Children); i++)
    {

        void *currentChild = parent->Children[i];

        if (child == currentChild)
        {
            arrdel(parent->Children, i);
            WAVE_GRAPHIC_OBJECT *CastedObject = (WAVE_GRAPHIC_OBJECT *)child;
            CastedObject->Parent = NULL;
            return TRUE;
        }
        
    }
    
    return FALSE;
}


void ObjectAddChild(WAVE_GRAPHIC_OBJECT *parent, void *child) {

    if (parent == child)
    {
        return;
    }
    
    WAVE_GRAPHIC_OBJECT *CastedObject = (WAVE_GRAPHIC_OBJECT *)child;


    if (!CastedObject)
    {
        return;
    }


    if(CastedObject->Parent != NULL) {
        ObjectRemoveChild(CastedObject->Parent, CastedObject);
    }
    
    arrput(parent->Children, child);
    CastedObject->Parent = parent;
}


static UINT32 NextObjectId = 0;

WAVE_GRAPHIC_OBJECT CreateEmptyObject(WAVE_CANVAS_MANAGER *canvas, VEC2 position, void (*render)(void *child))
{
    

    CHAR16 *ObjectGenericName = AllocatePool(300 * sizeof(CHAR16));

    SPrint(
        ObjectGenericName,
        sizeof(ObjectGenericName),
        L"<@object#%ld>",
        NextObjectId++
    );

    return (WAVE_GRAPHIC_OBJECT) {
        .Name = ObjectGenericName,
        .Render = render,
        .Children = NULL,
        .Canvas = canvas,
        
        .Visible = TRUE,
        .FinalVisible = TRUE,
        
        .Position = position,
        .Scale = VECTOR2_ONE,

        .FinalPosition = position,
        .FinalScale = VECTOR2_ONE
    };
}

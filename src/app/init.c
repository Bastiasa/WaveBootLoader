#include <graphics/canvas.h>
#include <graphics/elements/rectangle.h>
#include <graphics/elements/rounded_rectangle.h>
#include <graphics/elements/image.h>
#include <graphics/vector2.h>
#include <graphics/color.h>
#include <efi.h>
#include <efilib.h>
#include <mathutils.h>


#include <img/grub_icon.h>
#include <img/ubuntu_icon.h>
#include <img/windows_icon.h>

#include <img/windows_button.h>
#include <img/ubuntu_button.h>

#include <img/grub_button_focus.h>
#include <img/grub_button_normal.h>

#include <stb_ds.h>


#include "boot.h"

static WAVE_CANVAS_MANAGER *Canvas = NULL;

static inline void Add(void *child) {
    CanvasAdd(Canvas, child);
}

static inline BOOLEAN Remove(void *child) {
    return CanvasRemove(Canvas, child);
}

static WAVE_RND_RECT *WindowsButton;
static WAVE_RND_RECT *UbuntuButton;
static WAVE_IMAGE *GrubButton;

#define BTN_SPACING 30.0f

#define WINDOWS_BTN_SELECTED_COLOR 0x001749

#define WINDOWS_BTN_POSITION WV2( \
    Canvas->Width / 2.0f - 160.0f - BTN_SPACING, \ 
    Canvas->Height / 2.0f \
)

#define UBUNTU_BTN_SELECTED_COLOR 0x3d0a04

#define UBUNTU_BTN_POSITION WV2( \
    Canvas->Width / 2.0f + 150.0f + BTN_SPACING, \
    Canvas->Height / 2.0f \
)

#define INIT_DELAY_FRAMES 20 

enum BUTTON_FOCUS {
    NONE_SELECTED,
    WINDOWS_SELECTED,
    UBUNTU_SELECTED,
    GRUB_SELECTED
};

#define SELECTED_SCALE (VEC2) {.x = 1.1f, .y = 1.1f}
#define TRANSITION_WEIGHT 0.8f

enum BUTTON_FOCUS CurrentFocus = NONE_SELECTED;

static int IsKeyReleased(UINT32 key) {
    return Canvas->LastKey == key && Canvas->CurrentKey == SCAN_NULL;
}

static UINT32 GetReleasedKey() {
    return Canvas->LastKey;
}

WAVE_IMAGE *CreateIconForButton(
    WAVE_IMAGE_METADATA *meta
) {

    WAVE_IMAGE *icon = CreateImage(
        Canvas,
        meta,
        WV2(0, 0.0f),
        DIAGONAL_V2(256.0f)
    );

    icon->AnchorPoint = WV2(0.5f, .5f);

    return icon;
}


void CreateMainButtons() {
    Print(L"Creating buttons...\n");

    WindowsButton = CreateRoundedRectangle(
        Canvas,
        WV2(
            -150.0f, 
            Canvas->Height / 2.0f
        ),
        WV2(300, 300),
        8,
        0
    );


    UbuntuButton = CreateRoundedRectangle(
        Canvas,
        WV2(
            Canvas->Width + 150.0f, 
            Canvas->Height / 2.0f
        ),
        WV2(300.0f, 300.0f),
        8,
        0
    );
    
    WindowsButton->Object.Name = L"WindowsButton";
    UbuntuButton->Object.Name = L"UbuntuButton";

    Print(L"Buttons created\n");

    WindowsButton->AnchorPoint = DIAGONAL_V2(.5f);
    UbuntuButton->AnchorPoint = DIAGONAL_V2(.5);

    Add(UbuntuButton);
    Add(WindowsButton);

    WAVE_IMAGE *WindowsLogo = CreateIconForButton(
        &windows_button_metadata
    );

    WAVE_IMAGE *UbuntuLogo = CreateIconForButton(
        &ubuntu_button_metadata
    );

    ObjectAddChild(WindowsButton, WindowsLogo);
    ObjectAddChild(UbuntuButton, UbuntuLogo);
}

void CreateGrubButton() {
    GrubButton = CreateImage(
        Canvas,
        &grub_button_normal_metadata,

        WV2(Canvas->Width / 2, Canvas->Height + grub_button_normal_metadata.Width * .5),
        
        VectorMultiplyScalar(
            MetaToVector2(&grub_button_normal_metadata),
            0.5
        )
    );

    GrubButton->AnchorPoint = WV2(
        0.5f,
        1.0f
    );

    Add(GrubButton);

}

void InitUI(WAVE_CANVAS_MANAGER *GivenCanvas) {
    Canvas = GivenCanvas;

    CreateMainButtons();
    CreateGrubButton();
}


static int DelayCounter = 0;


static inline int isDelayPassed() {
    return DelayCounter > INIT_DELAY_FRAMES;
}

void UpdateMainButtons() {
    WindowsButton->Object.Scale = VectorLerp(
        WindowsButton->Object.Scale,
        CurrentFocus == WINDOWS_SELECTED ? SELECTED_SCALE : VECTOR2_ONE,
        TRANSITION_WEIGHT
    );

    WindowsButton->Color = WaveLerpColor(
        WindowsButton->Color,
        CurrentFocus == WINDOWS_SELECTED ? WINDOWS_BTN_SELECTED_COLOR : 0,
        0.15f
    );

    UbuntuButton->Object.Scale = VectorLerp(
        UbuntuButton->Object.Scale,
        CurrentFocus == UBUNTU_SELECTED ? SELECTED_SCALE : VECTOR2_ONE,
        TRANSITION_WEIGHT
    );

    UbuntuButton->Color = WaveLerpColor(
        UbuntuButton->Color,
        CurrentFocus == UBUNTU_SELECTED ? UBUNTU_BTN_SELECTED_COLOR : 0,
        0.15f
    );

    if (isDelayPassed())
    {
        UbuntuButton->Object.Position = VectorLerp(
            UbuntuButton->Object.Position,
            UBUNTU_BTN_POSITION,
            0.13f
        );

        WindowsButton->Object.Position = VectorLerp(
            WindowsButton->Object.Position,
            WINDOWS_BTN_POSITION,
            0.13f
        );
    }
    
}

void UpdateGrubButton() {
    GrubButton->ImageData = CurrentFocus == GRUB_SELECTED ?
        &grub_button_focus_metadata
        :
        &grub_button_normal_metadata;
    
    GrubButton->Object.Scale = VectorLerp(
        GrubButton->Object.Scale,
        CurrentFocus == GRUB_SELECTED ?
            SELECTED_SCALE
            :
            VECTOR2_ONE,
        0.3f
    );

    if (isDelayPassed())
    {
        GrubButton->Object.Position = VectorLerp(
            GrubButton->Object.Position,
            WV2(Canvas->Width / 2, Canvas->Height - 20.0f),
            0.13f
        );
    }
    
}


static WAVE_IMAGE *CurrentBootImage;

void SetCurrentBoot(WAVE_IMAGE_METADATA *meta) {
    HideObject(GrubButton);
    HideObject(WindowsButton);
    HideObject(UbuntuButton);

    CurrentBootImage = CreateImage(
        Canvas,
        meta,
        WV2(Canvas->Width / 2.0f, Canvas->Height / 2.0f),
        DIAGONAL_V2(300.0f)
    );

    CurrentBootImage->AnchorPoint = DIAGONAL_V2(.5f);
    Add(CurrentBootImage);
}

static BOOLEAN BootRequestAlreadyDone = FALSE;
static BOOLEAN BootRequestEnabled = TRUE;

void OnBootRequest() {

    if (!BootRequestEnabled)
    {
        return;
    }
    
    BootRequestAlreadyDone = TRUE;
    BootRequestEnabled = FALSE;

    switch (CurrentFocus)
    {
    case WINDOWS_SELECTED:
        SetCurrentBoot(&windows_icon_metadata);
        break;

    case UBUNTU_SELECTED:
        SetCurrentBoot(&ubuntu_icon_metadata);
        break;
    
    case GRUB_SELECTED:
        SetCurrentBoot(&grub_icon_metadata);
        break;
        
    default:
        break;
    }
}

void CheckBoot() {
    if (BootRequestAlreadyDone)
    {
        BootRequestAlreadyDone = FALSE;

        switch (CurrentFocus)
        {
        case WINDOWS_SELECTED:
            BootWindows(Canvas->ImageHandle);
            break;
        case UBUNTU_SELECTED:
            BootUbuntu(Canvas->ImageHandle);
            break;

        case GRUB_SELECTED:
            BootGrub(Canvas->ImageHandle);
            break;
        }
    }

}

void OnUpdate() {

    CheckBoot();


    if (!BootRequestEnabled)
    {
        return;
    }
    
    

    if (DelayCounter <= INIT_DELAY_FRAMES)
    {
        DelayCounter++;
    }


    if (IsKeyReleased(SCAN_LEFT))
    {
        CurrentFocus = WINDOWS_SELECTED;
    } else if (IsKeyReleased(SCAN_RIGHT)) {
        CurrentFocus = UBUNTU_SELECTED;
    } else if (IsKeyReleased(SCAN_DOWN)) {
        CurrentFocus = GRUB_SELECTED;
    }

    UpdateMainButtons();
    UpdateGrubButton();

    if (Canvas->UnicodeCurrentKey == CHAR_CARRIAGE_RETURN)
    {
        OnBootRequest();
    }
    
}

void OnBeforeRender() {

}

void OnAfterRender() {


}

void OnBeforeStep() {
    
}

void OnAfterStep() {

}



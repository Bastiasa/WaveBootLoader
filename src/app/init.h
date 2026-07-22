#ifndef WAVE_UI_INIT_DEF
#define WAVE_UI_INIT_DEF

void InitUI(WAVE_CANVAS_MANAGER *canvas);

void OnBeforeRender(void);
void OnAfterRender(void);
void OnBeforeStep(void);
void OnAfterStep(void);

void OnUpdate(void);

#endif
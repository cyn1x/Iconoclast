#ifndef APP_H
#define APP_H

#include "audio.h"
#include "renderer.h"

void GameUpdate(struct platform_graphics_buffer *graphicsBuffer,
                struct platform_sound_buffer    *soundBuffer);

#endif // !APP_H

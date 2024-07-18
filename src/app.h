#ifndef APP_H
#define APP_H

#include "audio.h"
#include "renderer.h"

void GameUpdate(struct output_graphics_buffer *graphicsBuffer,
                struct output_sound_buffer    *soundBuffer);

#endif // !APP_H

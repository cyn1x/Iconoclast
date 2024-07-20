#ifndef APP_H
#define APP_H

#include "audio.h"
#include "input.h"
#include "renderer.h"

void PlatformUpdate(struct platform_graphics_buffer *graphicsBuffer,
                    struct platform_sound_buffer    *soundBuffer,
                    struct platform_input           *controllerInput);

#endif // !APP_H

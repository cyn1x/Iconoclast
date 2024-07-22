#ifndef APP_H
#define APP_H

#include "audio.h"
#include "game.h"
#include "input.h"
#include "mem.h"
#include "renderer.h"

void PlatformUpdate(struct platform_graphics *graphics,
                    struct platform_audio *sound, struct platform_input *input);

#endif // !APP_H

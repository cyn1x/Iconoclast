#ifndef GAME_H
#define GAME_H

#include "audio.h"
#include "input.h"
#include "mem.h"
#include "renderer.h"

struct game_state
{
};

void GameUpdate(struct platform_graphics *graphics,
                struct platform_audio *sound, struct platform_input *input,
                platform_memory *memory);

#endif // !GAME_H

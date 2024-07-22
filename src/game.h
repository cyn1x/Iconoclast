#ifndef GAME_H
#define GAME_H

#include "audio.h"
#include "input.h"
#include "mem.h"
#include "renderer.h"

struct game_state
{
};

void GameUpdate(struct game_graphics *graphics, struct game_audio *sound,
                struct game_input *input, game_memory *memory);

#endif // !GAME_H

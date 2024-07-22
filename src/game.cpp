#include "game.h"
#include "defs.h"

void GameUpdate(struct game_graphics *graphics, struct game_audio *sound,
                struct game_input *input, game_memory *memory)
{
    Render(graphics);
    GameUpdateSound(sound);
    GameHandleInput(input);

    Assert(sizeof(game_state) <= memory->permanentStorageSize);

    game_state *game = (game_state *)memory->permanentStorage;
    if (!memory->isInitialized) {
        memory->isInitialized = true;
    }
}

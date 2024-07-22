#include "game.h"
#include "defs.h"

void GameUpdate(struct platform_graphics *graphics,
                struct platform_audio *sound, struct platform_input *input,
                platform_memory *memory)
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

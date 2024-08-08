#include "game.h"
#include "defs.h"

void Update(struct game_audio *sound, struct game_input *input,
            game_memory *memory)
{
    GameUpdateSound(sound);
    GameHandleInput(input);

    Assert(sizeof(game_state) <= memory->permanentStorageSize);

    game_state *game = (game_state *)memory->permanentStorage;
    if (!memory->isInitialized) {
        memory->isInitialized = true;
    }
}

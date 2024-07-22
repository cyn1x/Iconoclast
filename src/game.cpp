#include "game.h"
#include "defs.h"

void GameUpdate(platform_memory *memory) {
    Assert(sizeof(game_state) <= memory->permanentStorageSize);

    game_state *game = (game_state *)memory->permanentStorage;
    if (!memory->isInitialized) {
        memory->isInitialized = true;
    }
}

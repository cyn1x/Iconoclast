#ifndef WIN32_SYS_H
#define WIN32_SYS_H

#include "../audio.h"
#include "../mem.h"

void Win32AllocateMemory(game_memory *memory, game_audio *sound);
void Win32DeallocateMemory(game_memory *memory, game_audio *sound);

#endif // !WIN32_SYS_H

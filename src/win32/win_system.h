#ifndef WIN32_SYS_H
#define WIN32_SYS_H

#include "../audio.h"
#include "../mem.h"

void AllocateStorage(platform_memory *memory, platform_audio *sound);
void DeallocateStorage(platform_memory *memory, platform_audio *sound);

#endif // !WIN32_SYS_H

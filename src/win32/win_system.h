#ifndef WIN32_SYS_H
#define WIN32_SYS_H

#include "../audio.h"
#include "../mem.h"

void Win32AllocateMemory(platform_memory *memory, platform_audio *sound);
void Win32DeallocateMemory(platform_memory *memory, platform_audio *sound);

#endif // !WIN32_SYS_H

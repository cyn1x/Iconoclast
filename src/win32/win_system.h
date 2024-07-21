#ifndef WIN32_SYS_H
#define WIN32_SYS_H

#include "../audio.h"
#include "../mem.h"

void Win32AllocateStorage(platform_memory *memory, platform_audio *sound);
void Win32DeallocateStorage(platform_memory *memory, platform_audio *sound);

#endif // !WIN32_SYS_H

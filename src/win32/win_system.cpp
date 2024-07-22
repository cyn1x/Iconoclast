#include "win_system.h"
#include "../defs.h"
#include <windows.h>

void Win32Error(bool succeeded);

void Win32AllocateMemory(platform_memory *memory, platform_audio *sound)
{
#if DEBUG
    LPVOID baseAddress = (LPVOID)Terabytes((uint64)2);
#else
    LPVOID baseAddress = 0;
#endif

    memory->permanentStorageSize = Megabytes(64);
    memory->transientStorageSize = Gigabytes((uint64)4);

    uint64 totalSize =
        memory->permanentStorageSize + memory->transientStorageSize;

    LPVOID permanentStorage = VirtualAlloc(
        baseAddress, totalSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    LPVOID transientStorage =
        ((uint8 *)permanentStorage + memory->permanentStorageSize);

    LPVOID soundStorage = VirtualAlloc(
        0, sound->bufferSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    if (!soundStorage && !permanentStorage && !transientStorage) {
        // TODO: Handle allocation failure
    }

    sound->samples           = (int16 *)soundStorage;
    memory->permanentStorage = permanentStorage;
    memory->transientStorage = transientStorage;
}

void Win32DeallocateMemory(platform_memory *memory, platform_audio *sound)
{
    Win32Error(VirtualFree(sound->samples, 0, MEM_RELEASE));
    Win32Error(VirtualFree(memory->permanentStorage, 0, MEM_RELEASE));
}

void Win32Error(bool succeeded)
{
    if (!succeeded) {
        // TODO: Handle deallocation failure
        DWORD err = GetLastError();
    }
}

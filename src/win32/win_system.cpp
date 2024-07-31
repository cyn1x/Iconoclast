#include "win_system.h"
#include "../defs.h"
#include <windows.h>

void Win32Error(bool succeeded);

void Win32AllocateMemory(game_memory *memory, game_audio *sound)
{
#if DEBUG
    LPVOID baseAddress = (LPVOID)Terabytes((uint64)2);
#else
    LPVOID baseAddress = 0;
#endif

    // TODO: Dynamically determine 32-bit mode and obtain less than 4 GB of
    // memory
    memory->permanentStorageSize = Megabytes(64);
    memory->transientStorageSize = Gigabytes(2);

    size_t totalSize =
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

void Win32DeallocateMemory(game_memory *memory, game_audio *sound)
{
    Win32Error(VirtualFree(sound->samples, 0, MEM_RELEASE));
    Win32Error(VirtualFree(memory->permanentStorage, 0, MEM_RELEASE));
}

// TODO: Abstract into win_error.cpp for other translation units
void Win32Error(bool succeeded)
{
    if (!succeeded) {
        // TODO: Handle deallocation failure
        DWORD err = GetLastError();
    }
}

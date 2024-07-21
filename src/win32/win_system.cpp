#include "win_system.h"
#include <windows.h>

void Error(bool succeeded);

void AllocateStorage(platform_memory *memory, platform_audio *sound)
{
    LPVOID soundStorage = VirtualAlloc(
        0, sound->bufferSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    LPVOID permanentStorage =
        VirtualAlloc(0, memory->permanentStorageSize, MEM_RESERVE | MEM_COMMIT,
                     PAGE_READWRITE);
    LPVOID transientStorage =
        VirtualAlloc(0, memory->permanentStorageSize, MEM_RESERVE | MEM_COMMIT,
                     PAGE_READWRITE);

    if (!soundStorage && !permanentStorage && !transientStorage) {
        // TODO: Handle allocation failure
    }

    sound->samples = (int16 *)soundStorage;
    PlatformSetMemory(memory, permanentStorage, transientStorage);
}

void DeallocateStorage(platform_memory *memory, platform_audio *sound)
{
    Error(VirtualFree(sound->samples, 0, MEM_RELEASE));
    Error(VirtualFree(memory->permanentStorage, 0, MEM_RELEASE));
    Error(VirtualFree(memory->transientStorage, 0, MEM_RELEASE));
}

void Error(bool succeeded)
{
    if (!succeeded) {
        // TODO: Handle deallocation failure
        DWORD err = GetLastError();
    }
}

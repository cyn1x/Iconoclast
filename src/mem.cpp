#include "mem.h"
#include "defs.h"

void PlatformInitMemory(platform_memory *memory)
{
    memory->permanentStorageSize = Megabytes(64);
    memory->transientStorageSize = Gigabytes((uint64)4);
}

void PlatformSetMemory(platform_memory *memory, void *permanentStorage,
                       void *transientStorage)
{
    memory->permanentStorage = permanentStorage;
    memory->transientStorage = transientStorage;
}

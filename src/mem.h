#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

struct platform_memory
{
    uint64 permanentStorageSize;
    uint64 transientStorageSize;
    void  *permanentStorage;
    void  *transientStorage;
    bool   isInitialized;
};

void PlatformInitMemory(platform_memory *memory);
void PlatformSetMemory(platform_memory *memory, void *permanentStorage,
                       void *transientStorage);

#endif // !MEMORY_H

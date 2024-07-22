#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

struct game_memory
{
    uint64 permanentStorageSize;
    uint64 transientStorageSize;
    void  *permanentStorage;
    void  *transientStorage;
    bool   isInitialized;
};

#endif // !MEMORY_H

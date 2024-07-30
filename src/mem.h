#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

struct game_memory
{
    size_t permanentStorageSize;
    size_t transientStorageSize;
    void  *permanentStorage;
    void  *transientStorage;
    bool   isInitialized;
};

#endif // !MEMORY_H

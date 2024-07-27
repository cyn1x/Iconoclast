#ifndef PLATFORM_H
#define PLATFORM_H

#include "types.h"

struct blob
{
    uint32 size;
    void  *contents;
};

blob PlatformReadBlob(char *filename);
bool PlatformWriteBlob(char *filename, blob *blob);
void PlatformFreeBlob(void *memory);

#endif // !PLATFORM_H

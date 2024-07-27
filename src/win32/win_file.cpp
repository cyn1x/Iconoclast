#include "../platform.h"
#include "../util.h"
#include <windows.h>

blob PlatformReadBlob(char *filename)
{
    blob   blob       = {};
    HANDLE fileHandle = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, 0,
                                    OPEN_EXISTING, 0, 0);

    if (fileHandle == INVALID_HANDLE_VALUE) {
        // TODO: Handle invalid handle value
        return blob;
    }

    LARGE_INTEGER fileSize;
    if (!GetFileSizeEx(fileHandle, &fileSize)) {
        // TODO: Handle file size failure
        return blob;
    }
    uint32 fileSize32 = SafeTruncateUInt64(fileSize.QuadPart);

    // TODO: Change to HeapAlloc which is better suited for small allocations
    void *buffer =
        VirtualAlloc(0, fileSize32, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!buffer) {
        // TODO: Handle memory allocation failure
        return blob;
    }

    DWORD bytesRead;
    if (ReadFile(fileHandle, buffer, fileSize32, &bytesRead, 0) &&
        fileSize32 == bytesRead) {
        // File read success
        blob.size     = fileSize32;
        blob.contents = buffer;

    } else {
        PlatformFreeBlob(buffer);
        buffer = 0;
    }

    CloseHandle(fileHandle);

    return blob;
}

bool PlatformWriteBlob(char *filename, blob *blob)
{
    bool result = false;

    // TODO: Check if dir exists and create it if not
    HANDLE fileHandle =
        CreateFileA(filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);

    if (fileHandle == INVALID_HANDLE_VALUE) {
        // TODO: Handle invalid handle value
        return result;
    }

    DWORD bytesWritten;
    if (WriteFile(fileHandle, blob->contents, blob->size, &bytesWritten, 0)) {
        // File write successful
        result = bytesWritten == blob->size;
    } else {
        // TODO: Handle file write failure
    }

    CloseHandle(fileHandle);

    return result;
}

void PlatformFreeBlob(void *memory) { VirtualFree(memory, 0, MEM_RELEASE); }

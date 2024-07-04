#ifndef SOUND_H
#define SOUND_H

#include <dsound.h>
#include <stdint.h>

#define DIRECT_SOUND_CREATE(name)                                              \
    HRESULT WINAPI name(LPCGUID pcGuidDevice, LPDIRECTSOUND *ppDS,             \
                        LPUNKNOWN pUnkOuter)

typedef DIRECT_SOUND_CREATE(direct_sound_create);

void Win32InitDSound(HWND window, int32_t samplesPerSec, int32_t bufferSize);

#endif // SOUND_H

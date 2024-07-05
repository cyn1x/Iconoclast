#ifndef SOUND_H
#define SOUND_H

#include <dsound.h>
#include <stdint.h>

#define DIRECT_SOUND_CREATE(name)                                              \
    HRESULT WINAPI name(LPCGUID pcGuidDevice, LPDIRECTSOUND *ppDS,             \
                        LPUNKNOWN pUnkOuter)

typedef DIRECT_SOUND_CREATE(direct_sound_create);

static LPDIRECTSOUNDBUFFER SecondaryBuffer;

void                       Win32InitDSound(HWND window);
void                       Win32PlaySound(void);

#endif // SOUND_H

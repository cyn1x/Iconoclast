#ifndef WIN_SOUND_H
#define WIN_SOUND_H

#include "../audio.h"
#include <dsound.h>

#define DIRECT_SOUND_CREATE(name)                                              \
    HRESULT WINAPI name(LPCGUID pcGuidDevice, LPDIRECTSOUND *ppDS,             \
                        LPUNKNOWN pUnkOuter)

typedef DIRECT_SOUND_CREATE(direct_sound_create);

void Win32InitDSound(HWND window, struct platform_sound_buffer *platformBuffer);
void Win32UpdateAudio(struct platform_sound_buffer *platformBuffer);
void Win32UpdateSound(struct platform_sound_buffer *platformBuffer);

#endif // !WIN_SOUND_H

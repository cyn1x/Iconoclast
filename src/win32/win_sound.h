#ifndef WIN_SOUND_H
#define WIN_SOUND_H

#include "../audio.h"
#include <dsound.h>

#define DIRECT_SOUND_CREATE(name)                                              \
    HRESULT WINAPI name(LPCGUID pcGuidDevice, LPDIRECTSOUND *ppDS,             \
                        LPUNKNOWN pUnkOuter)

typedef DIRECT_SOUND_CREATE(direct_sound_create);

void Win32InitDSound(HWND window);
void Win32SetPlatformData(struct output_sound_buffer *buffer);
void Win32UpdateSound(struct output_sound_buffer *buffer);
void Win32WriteSoundBuffer(struct output_sound_buffer *buffer);

#endif // !WIN_SOUND_H

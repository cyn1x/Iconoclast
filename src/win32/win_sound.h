#ifndef WIN_SOUND_H
#define WIN_SOUND_H

#include "../audio.h"
#include <dsound.h>

#define DIRECT_SOUND_CREATE(name)                                              \
    HRESULT WINAPI name(LPCGUID pcGuidDevice, LPDIRECTSOUND *ppDS,             \
                        LPUNKNOWN pUnkOuter)

typedef DIRECT_SOUND_CREATE(direct_sound_create);

void Win32InitDSound(HWND window, struct game_audio *gameAudio);
void Win32UpdateDSound(struct game_audio *gameAudio);
void Win32FillSoundBuffer(struct game_audio *gameAudio);

#endif // !WIN_SOUND_H

#include "sound.h"

LPDIRECTSOUNDBUFFER soundBuffer;

uint32_t            sampleIndex         = 0;
uint32_t            runningSampleIndex  = 0;

int32_t             samplesPerSecond    = 48000;
int16_t             toneHz              = 256;
int16_t             toneVolume          = 3000;

int                 squareWavePeriod    = samplesPerSecond / toneHz;
int                 halfSquareWave      = squareWavePeriod / 2;
int                 bytesPerSample      = sizeof(int16_t) * 2;
int                 secondaryBufferSize = samplesPerSecond * bytesPerSample;

void                Win32WriteSoundBuffer(int16_t *region, DWORD regionSize);

void                Win32InitDSound(HWND window)
{
    HMODULE dSoundLibrary = LoadLibraryA("dsound.dll");
    if (!dSoundLibrary) {
        // Unable to load dsound.dll
        // TODO: Add logging
        return;
    }

    direct_sound_create *directSoundCreate =
        (direct_sound_create *)GetProcAddress(dSoundLibrary,
                                              "DirectSoundCreate");

    LPDIRECTSOUND directSound;
    HRESULT       createDirectSound = directSoundCreate(0, &directSound, 0);
    if (directSoundCreate && FAILED(createDirectSound)) {
        // Error creating and initializes an IDirectSound interface
        // TODO: Add logging
        return;
    }

    WAVEFORMATEX waveFormat   = {};
    waveFormat.wFormatTag     = WAVE_FORMAT_PCM;
    waveFormat.nChannels      = 2;
    waveFormat.nSamplesPerSec = samplesPerSecond;
    waveFormat.wBitsPerSample = 16;
    waveFormat.nBlockAlign =
        (waveFormat.nChannels * waveFormat.wBitsPerSample) / 8;
    waveFormat.nAvgBytesPerSec =
        waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
    waveFormat.cbSize = 0;

    HRESULT setCooperativeLevel =
        directSound->SetCooperativeLevel(window, DSSCL_PRIORITY);
    if (FAILED(setCooperativeLevel)) {
        // Error establishing the cooperative level for this instance of
        // the device
        // TODO: Add logging
        return;
    }

    DSBUFFERDESC bufferDescription = {};
    bufferDescription.dwSize       = sizeof(bufferDescription);
    bufferDescription.dwFlags      = DSBCAPS_PRIMARYBUFFER;

    LPDIRECTSOUNDBUFFER primaryBuffer;
    HRESULT             createSoundBuffer =
        directSound->CreateSoundBuffer(&bufferDescription, &primaryBuffer, 0);
    if (FAILED(createSoundBuffer)) {
        // Error creating primary DirectSoundBuffer
        // TODO: Add logging
        return;
    }
    // Handle to the sound card initialized

    HRESULT setFormat = primaryBuffer->SetFormat(&waveFormat);
    if (FAILED(setFormat)) {
        // Error setting the format of the primary buffer
        // TODO: Add logging
        return;
    }

    bufferDescription               = {};
    bufferDescription.dwSize        = sizeof(bufferDescription);
    bufferDescription.dwFlags       = 0;
    bufferDescription.dwBufferBytes = secondaryBufferSize;
    bufferDescription.lpwfxFormat   = &waveFormat;

    HRESULT createSecondaryBuffer =
        directSound->CreateSoundBuffer(&bufferDescription, &soundBuffer, 0);
    if (FAILED(createSecondaryBuffer)) {
        // Error creating secondary DirectSoundBuffer
        // TODO: Add logging
    }
}

void Win32PlaySound()
{
    if (!soundBuffer) {
        // SoundBuffer is null
        // TODO: Add logging
        return;
    }

    DWORD   playCursor;
    DWORD   writeCursor;
    HRESULT getCurrentPosition =
        soundBuffer->GetCurrentPosition(&playCursor, &writeCursor);
    if (FAILED(getCurrentPosition)) {
        // Failed to get the current position in the SoundBuffer
        // TODO: Add logging
        return;
    }

    DWORD byteToLock =
        runningSampleIndex * bytesPerSample % secondaryBufferSize;
    DWORD bytesToWrite;
    if (byteToLock == playCursor) {
        bytesToWrite = secondaryBufferSize;
    } else if (byteToLock > playCursor) {
        bytesToWrite = secondaryBufferSize - byteToLock;
        bytesToWrite += playCursor;
    } else {
        bytesToWrite = playCursor - byteToLock;
    }

    VOID *regionOne;
    DWORD regionOneSize;
    VOID *regionTwo;
    DWORD regionTwoSize;
    if (FAILED(soundBuffer->Lock(byteToLock, bytesToWrite, &regionOne,
                                 &regionOneSize, &regionTwo, &regionTwoSize,
                                 0))) {
        // Error locking SoundBuffer
        // TODO: Add logging
        return;
    }

    Win32WriteSoundBuffer((int16_t *)regionOne, regionOneSize);
    Win32WriteSoundBuffer((int16_t *)regionTwo, regionTwoSize);

    soundBuffer->Unlock(regionOne, regionOneSize, regionTwo, regionTwoSize);
    soundBuffer->Play(0, 0, DSBPLAY_LOOPING);
}

void Win32WriteSoundBuffer(int16_t *region, DWORD regionSize)
{
    DWORD    regionSampleCount = regionSize / bytesPerSample;
    int16_t *sampleOut         = region;

    sampleOut                  = region;

    for (DWORD sampleIndex = 0; sampleIndex < regionSampleCount;
         ++sampleIndex) {

        int16_t sampleValue = ((runningSampleIndex++ / halfSquareWave) % 2)
                                  ? toneVolume
                                  : -toneVolume;
        *sampleOut++        = sampleValue;
        *sampleOut++        = sampleValue;
    }
}

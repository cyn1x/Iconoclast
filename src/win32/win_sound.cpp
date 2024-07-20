#include "win_sound.h"
#include "../types.h"

struct win32_sound_output
{
    int    samplesPerSec;
    int    toneHz;
    int16  toneVolume;
    uint32 runningSampleIndex;
    int    bytesPerSample;
    int    secondaryBufferSize;
    int    latencySampleCount;
    DWORD  byteToLock;
    DWORD  bytesToWrite;
};

win32_sound_output  Win32SoundOutput = {};

LPDIRECTSOUNDBUFFER SoundBuffer;

void                Win32ClearSoundBuffer();

void                Win32InitSoundData()
{
    Win32SoundOutput.samplesPerSec      = 48000;
    Win32SoundOutput.toneHz             = 256;
    Win32SoundOutput.toneVolume         = 3000;
    Win32SoundOutput.runningSampleIndex = 0;
    Win32SoundOutput.bytesPerSample     = sizeof(int16) * 2;
    Win32SoundOutput.secondaryBufferSize =
        Win32SoundOutput.samplesPerSec * Win32SoundOutput.bytesPerSample;
    // FIX: Dynamically determine latency to avoid any output skipping
    Win32SoundOutput.latencySampleCount = Win32SoundOutput.samplesPerSec / 15;
}

void Win32InitPlatformSound(platform_sound_buffer *platformBuffer)
{
    int16 samples[48000 * sizeof(int16) * 2];

    platformBuffer->samples            = samples;
    platformBuffer->samplesPerSec      = Win32SoundOutput.samplesPerSec;
    platformBuffer->toneHz             = Win32SoundOutput.toneHz;
    platformBuffer->toneVolume         = Win32SoundOutput.toneVolume;
    platformBuffer->runningSampleIndex = Win32SoundOutput.runningSampleIndex;
    platformBuffer->bytesPerSample     = Win32SoundOutput.bytesPerSample;
}

void Win32InitDSound(HWND window, platform_sound_buffer *platformBuffer)
{
    Win32InitSoundData();

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
    waveFormat.nSamplesPerSec = Win32SoundOutput.samplesPerSec;
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
        // Error setting the format of the primary platformBuffer
        // TODO: Add logging
        return;
    }

    bufferDescription               = {};
    bufferDescription.dwSize        = sizeof(bufferDescription);
    bufferDescription.dwFlags       = 0;
    bufferDescription.dwBufferBytes = Win32SoundOutput.secondaryBufferSize;
    bufferDescription.lpwfxFormat   = &waveFormat;

    HRESULT createSecondaryBuffer =
        directSound->CreateSoundBuffer(&bufferDescription, &SoundBuffer, 0);
    if (FAILED(createSecondaryBuffer)) {
        // Error creating secondary DirectSoundBuffer
        // TODO: Add logging
    }

    Win32ClearSoundBuffer();
    Win32InitPlatformSound(platformBuffer);

    SoundBuffer->Play(0, 0, DSBPLAY_LOOPING);
}

void Win32UpdateAudio(platform_sound_buffer *platformBuffer)
{
    if (!SoundBuffer) {
        // SoundBuffer is null
        // TODO: Add logging
        return;
    }

    DWORD   playCursor  = 0;
    DWORD   writeCursor = 0;
    HRESULT getCurrentPosition =
        SoundBuffer->GetCurrentPosition(&playCursor, &writeCursor);
    if (FAILED(getCurrentPosition)) {
        // Failed to get the current position in the SoundBuffer
        // TODO: Add logging
        return;
    }

    DWORD byteToLock = Win32SoundOutput.runningSampleIndex *
                       Win32SoundOutput.bytesPerSample %
                       Win32SoundOutput.secondaryBufferSize;
    DWORD targetCursor = ((playCursor + (Win32SoundOutput.latencySampleCount *
                                         Win32SoundOutput.bytesPerSample)) %
                          Win32SoundOutput.secondaryBufferSize);
    DWORD bytesToWrite = 0;
    if (byteToLock > targetCursor) {
        bytesToWrite = Win32SoundOutput.secondaryBufferSize - byteToLock;
        bytesToWrite += targetCursor;
    } else {
        bytesToWrite = targetCursor - byteToLock;
    }

    platformBuffer->sampleCount =
        bytesToWrite / Win32SoundOutput.bytesPerSample;

    Win32SoundOutput.byteToLock   = byteToLock;
    Win32SoundOutput.bytesToWrite = bytesToWrite;
}

void Win32UpdateSound(platform_sound_buffer *platformBuffer)
{
    VOID *regionOne;
    DWORD regionOneSize;
    VOID *regionTwo;
    DWORD regionTwoSize;
    if (FAILED(SoundBuffer->Lock(
            Win32SoundOutput.byteToLock, Win32SoundOutput.bytesToWrite,
            &regionOne, &regionOneSize, &regionTwo, &regionTwoSize, 0))) {
        // Error locking SoundBuffer
        // TODO: Add logging
        return;
    }

    int16 *sampleIn = platformBuffer->samples;

    // TODO: Abstract these loops into a single loop inside a
    // separate function once the latency sample count is dynamic
    DWORD  regionCount = regionOneSize / Win32SoundOutput.bytesPerSample;
    int16 *regionOut   = (int16 *)regionOne;
    for (DWORD sampleIndex = 0; sampleIndex < regionCount; ++sampleIndex) {

        *regionOut++ = *sampleIn++;
        *regionOut++ = *sampleIn++;

        ++Win32SoundOutput.runningSampleIndex;
    }

    regionCount = regionTwoSize / Win32SoundOutput.bytesPerSample;
    regionOut   = (int16 *)regionTwo;
    for (DWORD sampleIndex = 0; sampleIndex < regionCount; ++sampleIndex) {

        *regionOut++ = *sampleIn++;
        *regionOut++ = *sampleIn++;

        ++Win32SoundOutput.runningSampleIndex;
    }

    SoundBuffer->Unlock(regionOne, regionOneSize, regionTwo, regionTwoSize);
}

void Win32ClearSoundBuffer()
{
    VOID *regionOne;
    DWORD regionOneSize;
    VOID *regionTwo;
    DWORD regionTwoSize;
    if (FAILED(SoundBuffer->Lock(0, Win32SoundOutput.secondaryBufferSize,
                                 &regionOne, &regionOneSize, &regionTwo,
                                 &regionTwoSize, 0))) {
        // Error locking SoundBuffer
        // TODO: Add logging
        return;
    }

    DWORD regionCount = regionOneSize / Win32SoundOutput.bytesPerSample;
    int8 *regionOut   = (int8 *)regionOne;
    for (DWORD sampleIndex = 0; sampleIndex < regionCount; ++sampleIndex) {
        *regionOut++ = 0;
    }

    regionCount = regionTwoSize / Win32SoundOutput.bytesPerSample;
    regionOut   = (int8 *)regionTwo;
    for (DWORD sampleIndex = 0; sampleIndex < regionCount; ++sampleIndex) {
        *regionOut++ = 0;
    }

    SoundBuffer->Unlock(regionOne, regionOneSize, regionTwo, regionTwoSize);
}

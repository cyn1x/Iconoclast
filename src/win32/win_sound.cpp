#include "win_sound.h"
#include "types.h"

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

win32_sound_output  SoundOutput = {};

LPDIRECTSOUNDBUFFER SoundBuffer;

void                Win32ClearSoundBuffer();

void                Win32InitSoundData()
{
    SoundOutput.samplesPerSec      = 48000;
    SoundOutput.toneHz             = 256;
    SoundOutput.toneVolume         = 3000;
    SoundOutput.runningSampleIndex = 0;
    SoundOutput.bytesPerSample     = sizeof(int16) * 2;
    SoundOutput.secondaryBufferSize =
        SoundOutput.samplesPerSec * SoundOutput.bytesPerSample;
    SoundOutput.latencySampleCount = SoundOutput.samplesPerSec / 15;
}

void Win32SetPlatformData(output_sound_buffer *buffer)
{
    int16 samples[48000 * sizeof(int16) * 2];

    buffer->samples            = samples;
    buffer->samplesPerSec      = SoundOutput.samplesPerSec;
    buffer->toneHz             = SoundOutput.toneHz;
    buffer->toneVolume         = SoundOutput.toneVolume;
    buffer->runningSampleIndex = SoundOutput.runningSampleIndex;
    buffer->bytesPerSample     = SoundOutput.bytesPerSample;
}

void Win32InitDSound(HWND window)
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
    waveFormat.nSamplesPerSec = SoundOutput.samplesPerSec;
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
    bufferDescription.dwBufferBytes = SoundOutput.secondaryBufferSize;
    bufferDescription.lpwfxFormat   = &waveFormat;

    HRESULT createSecondaryBuffer =
        directSound->CreateSoundBuffer(&bufferDescription, &SoundBuffer, 0);
    if (FAILED(createSecondaryBuffer)) {
        // Error creating secondary DirectSoundBuffer
        // TODO: Add logging
    }

    Win32ClearSoundBuffer();

    SoundBuffer->Play(0, 0, DSBPLAY_LOOPING);
}

void Win32UpdateSound(output_sound_buffer *buffer)
{
    if (!SoundBuffer) {
        // SoundBuffer is null
        // TODO: Add logging
        return;
    }

    DWORD   playCursor;
    DWORD   writeCursor;
    HRESULT getCurrentPosition =
        SoundBuffer->GetCurrentPosition(&playCursor, &writeCursor);
    if (FAILED(getCurrentPosition)) {
        // Failed to get the current position in the SoundBuffer
        // TODO: Add logging
        return;
    }

    DWORD byteToLock = SoundOutput.runningSampleIndex *
                       SoundOutput.bytesPerSample %
                       SoundOutput.secondaryBufferSize;
    DWORD targetCursor = ((playCursor + (SoundOutput.latencySampleCount *
                                         SoundOutput.bytesPerSample)) %
                          SoundOutput.secondaryBufferSize);
    DWORD bytesToWrite;
    if (byteToLock > targetCursor) {
        bytesToWrite = SoundOutput.secondaryBufferSize - byteToLock;
        bytesToWrite += targetCursor;
    } else {
        bytesToWrite = targetCursor - byteToLock;
    }

    buffer->sampleCount      = bytesToWrite / SoundOutput.bytesPerSample;

    SoundOutput.byteToLock   = byteToLock;
    SoundOutput.bytesToWrite = bytesToWrite;
}

void Win32WriteSoundBuffer(output_sound_buffer *buffer)
{
    VOID *regionOne;
    DWORD regionOneSize;
    VOID *regionTwo;
    DWORD regionTwoSize;
    if (FAILED(SoundBuffer->Lock(
            SoundOutput.byteToLock, SoundOutput.bytesToWrite, &regionOne,
            &regionOneSize, &regionTwo, &regionTwoSize, 0))) {
        // Error locking SoundBuffer
        // TODO: Add logging
        return;
    }

    int16 *sampleIn = buffer->samples;

    // FIX: Find out why abstracting these loops into a single loop inside a
    // separate function causes the sound to skip
    DWORD  regionCount = regionOneSize / SoundOutput.bytesPerSample;
    int16 *regionOut   = (int16 *)regionOne;
    for (DWORD sampleIndex = 0; sampleIndex < regionCount; ++sampleIndex) {

        *regionOut++ = *sampleIn++;
        *regionOut++ = *sampleIn++;

        ++SoundOutput.runningSampleIndex;
    }

    regionCount = regionTwoSize / SoundOutput.bytesPerSample;
    regionOut   = (int16 *)regionTwo;
    for (DWORD sampleIndex = 0; sampleIndex < regionCount; ++sampleIndex) {

        *regionOut++ = *sampleIn++;
        *regionOut++ = *sampleIn++;

        ++SoundOutput.runningSampleIndex;
    }

    SoundBuffer->Unlock(regionOne, regionOneSize, regionTwo, regionTwoSize);
}

void Win32ClearSoundBuffer()
{
    VOID *regionOne;
    DWORD regionOneSize;
    VOID *regionTwo;
    DWORD regionTwoSize;
    if (FAILED(SoundBuffer->Lock(0, SoundOutput.secondaryBufferSize, &regionOne,
                                 &regionOneSize, &regionTwo, &regionTwoSize,
                                 0))) {
        // Error locking SoundBuffer
        // TODO: Add logging
        return;
    }

    DWORD regionCount = regionOneSize / SoundOutput.bytesPerSample;
    int8 *regionOut   = (int8 *)regionOne;
    for (DWORD sampleIndex = 0; sampleIndex < regionCount; ++sampleIndex) {
        *regionOut++ = 0;
    }

    regionCount = regionTwoSize / SoundOutput.bytesPerSample;
    regionOut   = (int8 *)regionTwo;
    for (DWORD sampleIndex = 0; sampleIndex < regionCount; ++sampleIndex) {
        *regionOut++ = 0;
    }

    SoundBuffer->Unlock(regionOne, regionOneSize, regionTwo, regionTwoSize);
}

#include "sound.h"

struct win32_sound_output
{
    int      samplesPerSecond;
    int      toneHz;
    int16_t  toneVolume;
    uint32_t runningSampleIndex;
    int      bytesPerSample;
    int      secondaryBufferSize;
    int      latencySampleCount;
};

struct square_wave
{
    int squareWavePeriod;
    int halfSquareWave;
};

struct sine_wave
{
    float tSine;
    int   wavePeriod;
};

// TODO: Create Triangle Wave
struct triangle_wave
{
};

// TODO: Create Sawtooth Wave
struct sawtooth_wave
{
};

win32_sound_output  SoundOutput  = {};

square_wave         SquareWave   = {};
sine_wave           SineWave     = {};
triangle_wave       TriangleWave = {};
sawtooth_wave       Sawtoothave  = {};

LPDIRECTSOUNDBUFFER SoundBuffer;

void                Win32FillSoundBuffer(int16_t *region, DWORD regionSize);
int16_t             CalculateSineWave();
int16_t             CalculateSquareWave();
int16_t             CalculateTriangleWave();
int16_t             CalculateSawtoothWave();

void                InitializeSoundOutput()
{
    SoundOutput.samplesPerSecond = 48000, SoundOutput.toneHz = 256;
    SoundOutput.toneVolume         = 3000;
    SoundOutput.runningSampleIndex = 0;
    SoundOutput.bytesPerSample     = sizeof(int16_t) * 2;
    SoundOutput.secondaryBufferSize =
        SoundOutput.samplesPerSecond * SoundOutput.bytesPerSample;
    SoundOutput.latencySampleCount = SoundOutput.samplesPerSecond / 15;

    SquareWave.squareWavePeriod =
        SoundOutput.samplesPerSecond / SoundOutput.toneHz;
    SquareWave.halfSquareWave = SquareWave.squareWavePeriod / 2;

    SineWave.tSine            = 0;
    SineWave.wavePeriod = SoundOutput.samplesPerSecond / SoundOutput.toneHz;
}

void Win32InitDSound(HWND window)
{
    InitializeSoundOutput();

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
    waveFormat.nSamplesPerSec = SoundOutput.samplesPerSecond;
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

    SoundBuffer->Play(0, 0, DSBPLAY_LOOPING);
}

void Win32PlaySound()
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

    VOID *regionOne;
    DWORD regionOneSize;
    VOID *regionTwo;
    DWORD regionTwoSize;
    if (FAILED(SoundBuffer->Lock(byteToLock, bytesToWrite, &regionOne,
                                 &regionOneSize, &regionTwo, &regionTwoSize,
                                 0))) {
        // Error locking SoundBuffer
        // TODO: Add logging
        return;
    }

    Win32FillSoundBuffer((int16_t *)regionOne, regionOneSize);
    Win32FillSoundBuffer((int16_t *)regionTwo, regionTwoSize);

    SoundBuffer->Unlock(regionOne, regionOneSize, regionTwo, regionTwoSize);
}

void Win32FillSoundBuffer(int16_t *region, DWORD regionSize)
{
    DWORD    regionSampleCount = regionSize / SoundOutput.bytesPerSample;
    int16_t *sampleOut         = region;

    for (DWORD sampleIndex = 0; sampleIndex < regionSampleCount;
         ++sampleIndex) {

        float sampleValue = CalculateSineWave();

        *sampleOut++      = sampleValue;
        *sampleOut++      = sampleValue;

        SoundOutput.runningSampleIndex++;
    }
}

int16_t CalculateSineWave()
{
    float   sineValue   = sinf(SineWave.tSine);
    int16_t sampleValue = (int16_t)(sineValue * SoundOutput.toneVolume);
    SineWave.tSine += 2.0f * M_PI * 1.0f / (float)SineWave.wavePeriod;

    return sampleValue;
}

int16_t CalculateSquareWave()
{
    int16_t sampleValue =
        ((SoundOutput.runningSampleIndex++ / SquareWave.halfSquareWave) % 2)
            ? SoundOutput.toneVolume
            : -SoundOutput.toneVolume;

    return sampleValue;
}

#include "sound.h"

LPDIRECTSOUNDBUFFER SecondaryBuffer;

uint32_t            sampleIndex         = 0;
uint32_t            runningSampleIndex  = 0;

int32_t             samplesPerSecond    = 48000;
int16_t             toneHz              = 256;
int16_t             toneVolume          = 3000;

int                 squareWavePeriod    = samplesPerSecond / toneHz;
int                 halfSquareWave      = squareWavePeriod / 2;
int                 bytesPerSample      = sizeof(int16_t) * 2;
int                 secondaryBufferSize = samplesPerSecond * bytesPerSample;

void                Win32InitDSound(HWND window)
{
    HMODULE dSoundLibrary = LoadLibraryA("dsound.dll");

    if (dSoundLibrary) {
        direct_sound_create *directSoundCreate =
            (direct_sound_create *)GetProcAddress(dSoundLibrary,
                                                  "DirectSoundCreate");

        LPDIRECTSOUND directSound;
        if (directSoundCreate &&
            SUCCEEDED(directSoundCreate(0, &directSound, 0))) {

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

            if (SUCCEEDED(
                    directSound->SetCooperativeLevel(window, DSSCL_PRIORITY))) {

                DSBUFFERDESC bufferDescription = {};
                bufferDescription.dwSize       = sizeof(bufferDescription);
                bufferDescription.dwFlags      = DSBCAPS_PRIMARYBUFFER;

                LPDIRECTSOUNDBUFFER primaryBuffer;
                if (SUCCEEDED(directSound->CreateSoundBuffer(
                        &bufferDescription, &primaryBuffer, 0))) {
                    // Handle to the sound card initialized
                    HRESULT setFormat = primaryBuffer->SetFormat(&waveFormat);
                    if (SUCCEEDED(setFormat)) {
                        // Set the format of the primary buffer
                        //
                    } else {
                        // Error setting the format of the primary buffer
                        // TODO: Add logging
                    }
                } else {
                    // Error creating DirectSoundBuffer
                    // TODO: Add logging
                }
            } else {
                // Error establishing the cooperative level for this instance of
                // the device
                // TODO: Add logging
            }

            DSBUFFERDESC bufferDescription  = {};
            bufferDescription.dwSize        = sizeof(bufferDescription);
            bufferDescription.dwFlags       = 0;
            bufferDescription.dwBufferBytes = secondaryBufferSize;
            bufferDescription.lpwfxFormat   = &waveFormat;
            // bufferDescription.lpwfxFormat  = &waveFormat;

            if (SUCCEEDED(directSound->CreateSoundBuffer(
                    &bufferDescription, &SecondaryBuffer, 0))) {

                // HRESULT setFormat = SecondaryBuffer->SetFormat(&waveFormat);

            } else {
            }

        } else {
            // Error creating and initializes an IDirectSound interface
            // TODO: Add logging
        }
    } else {
        // Unable to load dsound.dll
        // TODO: Add logging
    }
}

void Win32PlaySound()
{
    DWORD playCursor;
    DWORD writeCursor;
    if (SUCCEEDED(
            SecondaryBuffer->GetCurrentPosition(&playCursor, &writeCursor))) {

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
        if (SUCCEEDED(SecondaryBuffer->Lock(byteToLock, bytesToWrite,
                                            &regionOne, &regionOneSize,
                                            &regionTwo, &regionTwoSize, 0))) {

            DWORD    regionOneSampleCount = regionOneSize / bytesPerSample;
            int16_t *sampleOut            = (int16_t *)regionOne;
            for (DWORD sampleIndex = 0; sampleIndex < regionOneSampleCount;
                 ++sampleIndex) {

                int16_t sampleValue =
                    ((runningSampleIndex++ / halfSquareWave) % 2) ? toneVolume
                                                                  : -toneVolume;
                *sampleOut++ = sampleValue;
                *sampleOut++ = sampleValue;
            }

            DWORD regionTwoSampleCount = regionTwoSize / bytesPerSample;
            sampleOut                  = (int16_t *)regionTwo;
            for (DWORD sampleIndex = 0; sampleIndex < regionTwoSampleCount;
                 ++sampleIndex) {

                int16_t sampleValue =
                    ((runningSampleIndex++ / halfSquareWave) % 2) ? toneVolume
                                                                  : -toneVolume;
                *sampleOut++ = sampleValue;
                *sampleOut++ = sampleValue;
            }

            SecondaryBuffer->Unlock(regionOne, regionOneSize, regionTwo,
                                    regionTwoSize);
        }
    }

    SecondaryBuffer->Play(0, 0, DSBPLAY_LOOPING);
}

#include "sound.h"

void Win32InitDSound(HWND window, int32_t samplesPerSec, int32_t bufferSize)
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
            waveFormat.wFormatTag     = 0;
            waveFormat.nChannels      = 2;
            waveFormat.nSamplesPerSec = samplesPerSec;
            waveFormat.wBitsPerSample = 16;
            waveFormat.nBlockAlign =
                (waveFormat.nChannels * waveFormat.wBitsPerSample) / 8;
            waveFormat.nAvgBytesPerSec =
                waveFormat.nChannels * waveFormat.wBitsPerSample;
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

            DSBUFFERDESC bufferDescription = {};
            bufferDescription.dwSize       = sizeof(bufferDescription);
            bufferDescription.dwFlags      = DSBCAPS_PRIMARYBUFFER;
            bufferDescription.lpwfxFormat  = &waveFormat;

            LPDIRECTSOUNDBUFFER secondaryBuffer;
            if (SUCCEEDED(directSound->CreateSoundBuffer(
                    &bufferDescription, &secondaryBuffer, 0))) {

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

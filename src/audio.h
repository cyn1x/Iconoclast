#ifndef AUDIO_H
#define AUDIO_H
#define _USE_MATH_DEFINES

#include "types.h"

struct output_sound_buffer
{
    int    samplesPerSec;
    int    sampleCount;
    int    bytesPerSample;
    int    toneHz;
    int16  toneVolume;
    int16 *samples;
    uint32 runningSampleIndex;
};

void OutputSound(struct output_sound_buffer *buffer);
void PlatformInitAudio(struct output_sound_buffer *buffer);

#endif // !AUDIO_H

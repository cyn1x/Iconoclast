#ifndef AUDIO_H
#define AUDIO_H
#define _USE_MATH_DEFINES

#include "types.h"

struct platform_audio
{
    int    bufferSize;
    int    samplesPerSec;
    int    sampleCount;
    int    bytesPerSample;
    int    toneHz;
    int16  toneVolume;
    int16 *samples;
    uint32 runningSampleIndex;
};

void OutputSound(struct platform_audio *audio);
void PlatformInitAudio(struct platform_audio *audio);

#endif // !AUDIO_H

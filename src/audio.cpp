#include "audio.h"
#include <math.h>

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

square_wave   SquareWave   = {};
sine_wave     SineWave     = {};
triangle_wave TriangleWave = {};
sawtooth_wave Sawtoothave  = {};

static int16  CalculateSineWave(platform_sound_buffer *buffer);
static int16  CalculateSquareWave(platform_sound_buffer *buffer);
static int16  CalculateTriangleWave(platform_sound_buffer *buffer);
static int16  CalculateSawtoothWave(platform_sound_buffer *buffer);

void          PlatformInitAudio(platform_sound_buffer *buffer)
{
    SquareWave.squareWavePeriod = buffer->samplesPerSec / buffer->toneHz;
    SquareWave.halfSquareWave   = SquareWave.squareWavePeriod / 2;

    SineWave.tSine              = 0;
    SineWave.wavePeriod         = buffer->samplesPerSec / buffer->toneHz;
}

void OutputSound(platform_sound_buffer *buffer)
{
    int16 *sampleOut = buffer->samples;

    for (int sampleIndex = 0; sampleIndex < buffer->sampleCount;
         ++sampleIndex) {

        float sampleValue = CalculateSineWave(buffer);

        *sampleOut++      = sampleValue;
        *sampleOut++      = sampleValue;

        buffer->runningSampleIndex++;
    }
}

int16_t CalculateSineWave(platform_sound_buffer *buffer)
{
    float sineValue   = sinf(SineWave.tSine);
    int16 sampleValue = (int16)(sineValue * buffer->toneVolume);
    SineWave.tSine += 2.0f * M_PI * 1.0f / (float)SineWave.wavePeriod;

    return sampleValue;
}

int16_t CalculateSquareWave(platform_sound_buffer *buffer)
{
    int   squareWavePeriod = buffer->samplesPerSec / buffer->toneHz;
    int   halfSquareWave   = squareWavePeriod / 2;

    int16 sampleValue      = ((buffer->runningSampleIndex / halfSquareWave) % 2)
                                 ? buffer->toneVolume
                                 : -buffer->toneVolume;

    return sampleValue;
}

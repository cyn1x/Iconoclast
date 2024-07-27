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

static int16  CalculateSineWave(game_audio *audio);
static int16  CalculateSquareWave(game_audio *audio);
static int16  CalculateTriangleWave(game_audio *audio);
static int16  CalculateSawtoothWave(game_audio *audio);

void          GameInitAudio(game_audio *audio)
{
    audio->toneHz               = 256;
    audio->toneVolume           = 3000;

    SquareWave.squareWavePeriod = audio->samplesPerSec / audio->toneHz;
    SquareWave.halfSquareWave   = SquareWave.squareWavePeriod / 2;

    SineWave.tSine              = 0;
    SineWave.wavePeriod         = audio->samplesPerSec / audio->toneHz;
}

void GameUpdateSound(game_audio *audio)
{
    int16 *sampleOut = audio->samples;

    for (int sampleIndex = 0; sampleIndex < audio->sampleCount; ++sampleIndex) {

        float sampleValue = CalculateSineWave(audio);

        *sampleOut++      = (int16)sampleValue;
        *sampleOut++      = (int16)sampleValue;

        audio->runningSampleIndex++;
    }
}

int16_t CalculateSineWave(game_audio *audio)
{
    float sineValue   = sinf(SineWave.tSine);
    int16 sampleValue = (int16)(sineValue * audio->toneVolume);
    SineWave.tSine += (float)(2.0f * M_PI * 1.0f / (float)SineWave.wavePeriod);

    return sampleValue;
}

int16_t CalculateSquareWave(game_audio *audio)
{
    int   squareWavePeriod = audio->samplesPerSec / audio->toneHz;
    int   halfSquareWave   = squareWavePeriod / 2;

    int16 sampleValue      = ((audio->runningSampleIndex / halfSquareWave) % 2)
                                 ? audio->toneVolume
                                 : -audio->toneVolume;

    return sampleValue;
}

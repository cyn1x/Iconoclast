#include "app.h"
#include "audio.h"

void GameUpdate(struct output_graphics_buffer *graphicsBuffer,
                struct output_sound_buffer    *soundBuffer)
{
    Render(graphicsBuffer);
    OutputSound(soundBuffer);
}

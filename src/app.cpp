#include "app.h"
#include "audio.h"

void GameUpdate(struct platform_graphics_buffer *graphicsBuffer,
                struct platform_sound_buffer    *soundBuffer)
{
    Render(graphicsBuffer);
    OutputSound(soundBuffer);
}

#include "app.h"
#include "audio.h"
#include "input.h"

void PlatformUpdate(struct platform_graphics_buffer *graphicsBuffer,
                    struct platform_sound_buffer    *soundBuffer,
                    struct platform_input           *controllerInput)
{
    Render(graphicsBuffer);
    OutputSound(soundBuffer);
    HandleInput(controllerInput);
}

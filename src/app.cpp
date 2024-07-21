#include "app.h"

void PlatformUpdate(struct platform_graphics *graphics,
                    struct platform_audio *sound, struct platform_input *input)
{
    Render(graphics);
    OutputSound(sound);
    HandleInput(input);
}

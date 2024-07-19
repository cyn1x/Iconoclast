#ifndef RENDERER_H
#define RENDERER_H

struct platform_graphics_buffer
{
    void *memory;
    int   width;
    int   height;
    int   pitch;
};

void Render(struct platform_graphics_buffer *buffer);

#endif // !RENDERER_H

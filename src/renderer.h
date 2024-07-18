#ifndef RENDERER_H
#define RENDERER_H

struct output_graphics_buffer
{
    void *memory;
    int   width;
    int   height;
    int   pitch;
};

void Render(struct output_graphics_buffer *buffer);

#endif // !RENDERER_H

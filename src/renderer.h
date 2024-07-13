#ifndef RENDERER_H
#define RENDERER_H

struct offscreen_buffer
{
    void *memory;
    int   width;
    int   height;
    int   pitch;
};

void Render(struct offscreen_buffer *buffer);

#endif // !RENDERER_H

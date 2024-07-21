#ifndef RENDERER_H
#define RENDERER_H

struct platform_graphics
{
    void *buffer;
    int   width;
    int   height;
    int   pitch;
};

void Render(struct platform_graphics *graphics);

#endif // !RENDERER_H

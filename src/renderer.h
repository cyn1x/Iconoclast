#ifndef RENDERER_H
#define RENDERER_H

struct game_graphics
{
    void *buffer;
    int   width;
    int   height;
    int   pitch;
};

void Render(struct game_graphics *graphics);

#endif // !RENDERER_H

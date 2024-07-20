#ifndef INPUT_H
#define INPUT_H

struct platform_button_state
{
    int  halfTransitionCount;
    bool endedDown;
};

struct platform_controller_input
{
    bool  isAnalog;

    float startX;
    float startY;

    float minX;
    float minY;

    float maxX;
    float maxY;

    float endX;
    float endY;

    union
    {
        struct platform_button_state buttons[6];
        struct
        {
            struct platform_button_state up;
            struct platform_button_state down;
            struct platform_button_state left;
            struct platform_button_state right;
            struct platform_button_state leftShoulder;
            struct platform_button_state rightShoulder;
        };
    };
};

struct platform_input
{
    struct platform_controller_input controllers[4];
};

void HandleInput(struct platform_input *Input);

#endif // !INPUT_H

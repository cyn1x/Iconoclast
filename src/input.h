#ifndef INPUT_H
#define INPUT_H

struct game_button_state
{
    int  halfTransitionCount;
    bool endedDown;
};

struct game_controller_input
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
        struct game_button_state buttons[6];
        struct
        {
            struct game_button_state up;
            struct game_button_state down;
            struct game_button_state left;
            struct game_button_state right;
            struct game_button_state leftShoulder;
            struct game_button_state rightShoulder;
        };
    };
};

struct game_input
{
    struct game_controller_input controllers[4];
};

void GameHandleInput(struct game_input *Input);

#endif // !INPUT_H

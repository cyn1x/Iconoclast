#ifndef INPUT_H
#define INPUT_H

struct game_button_state
{
    int  halfTransitionCount;
    bool endedDown;
};

struct game_controller_input
{
    bool isConnected;
    bool isAnalog;

    // Stick magnitude values
    float stickLX;
    float stickLY;
    float stickRX;
    float stickRY;

    union
    {
        struct game_button_state buttons[12];
        struct
        {
            struct game_button_state actionUp;
            struct game_button_state actionDown;
            struct game_button_state actionLeft;
            struct game_button_state actionRight;

            struct game_button_state moveUp;
            struct game_button_state moveDown;
            struct game_button_state moveLeft;
            struct game_button_state moveRight;

            struct game_button_state leftShoulder;
            struct game_button_state rightShoulder;

            struct game_button_state start;
            struct game_button_state back;

            // NOTE: Add new buttons before this declaration
            struct game_button_state defaultButton;
        };
    };
};

struct game_input
{
    struct game_controller_input controllers[5];
};

void                   GameHandleInput(struct game_input *input);
game_controller_input *GetController(game_input *input, int controllerIndex);

#endif // !INPUT_H

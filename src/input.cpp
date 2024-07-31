#include "input.h"
#include "defs.h"

void GameHandleInput(struct game_input *input)
{
    for (int controllerIndex = 0;
         controllerIndex <
         sizeof(input->controllers) / sizeof(input->controllers[0]);
         controllerIndex++) {

        struct game_controller_input *controller =
            &input->controllers[controllerIndex];
        if (controller->isAnalog) {
            // Use analog movement tuning
        } else {
            // Use digital movement tuning
        }
    }
}

game_controller_input *GetController(game_input *input, int controllerIndex)
{
    Assert((&input->controllers[0].defaultButton -
            &input->controllers[0].buttons[0]) ==
           sizeof(input->controllers[0].buttons) /
               sizeof(input->controllers[0].buttons[0]));
    Assert(controllerIndex <
           sizeof(input->controllers) / sizeof(input->controllers[0]));

    game_controller_input *result = &input->controllers[controllerIndex];

    return result;
}

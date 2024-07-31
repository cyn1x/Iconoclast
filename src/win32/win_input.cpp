#include "win_input.h"
#include "../defs.h"
#include "../input.h"
#include "../types.h"
#include "win_window.h"
#include <cmath>

X_INPUT_GET_STATE(XInputGetStateStub) { return ERROR_DEVICE_NOT_CONNECTED; }

X_INPUT_SET_STATE(XInputSetStateStub) { return ERROR_DEVICE_NOT_CONNECTED; }

static x_input_get_state     *XInputGetState_ = XInputGetStateStub;
static x_input_set_state     *XInputSetState_ = XInputSetStateStub;

static game_input             inputStates[2]  = {};
static game_input            *newInput        = &inputStates[0];
static game_input            *oldInput        = &inputStates[1];

static game_controller_input *oldController;
static game_controller_input *newController;

static void  Win32ProcessKeyboardMessage(game_button_state *newState,
                                         bool               isDown);
static void  Win32ProcessXInputDigitalButton(DWORD XInputButtonState,
                                             game_button_state *oldState,
                                             game_button_state *newState,
                                             DWORD              buttonBit);
static float Win32HandleStickInput(float x, float y, SHORT deadZoneThreshold);
static void  UpdateInputState();

void         Win32InitXInput(void)
{
    HMODULE XInputLibrary = LoadLibraryA("xinput1_4.dll");
    if (!XInputLibrary) {
        XInputLibrary = LoadLibraryA("xinput9_1_0.dll");
    }

    if (!XInputLibrary) {
        XInputLibrary = LoadLibraryA("xinput1_3.dll");
    }

    if (XInputLibrary) {
        XInputGetState = (x_input_get_state *)GetProcAddress(XInputLibrary,
                                                             "XInputGetState");
        if (!XInputGetState) {
            XInputGetState = XInputGetStateStub;
        }

        XInputSetState = (x_input_set_state *)GetProcAddress(XInputLibrary,
                                                             "XInputSetState");
        if (!XInputSetState) {
            XInputSetState = XInputSetStateStub;
        }
    }
}

void Win32UpdateXInput(game_input *input)
{
    DWORD dwResult;
    DWORD maxControllerCount = XUSER_MAX_COUNT;
    // Set maximum controller count to what is defined in input if
    // XUSER_MAX_COUNT exceeds what input supports.
    if (maxControllerCount >
        sizeof(input->controllers) / sizeof(input->controllers[0]) - 1) {

        maxControllerCount =
            sizeof(input->controllers) / sizeof(input->controllers[0]);
    }

    for (DWORD controllerIndex = 0; controllerIndex < maxControllerCount;
         controllerIndex++) {
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));

        // Keyboard will have an index of 0
        DWORD gamepadControllerIndex = controllerIndex + 1;
        oldController = GetController(input, gamepadControllerIndex);
        newController = GetController(input, gamepadControllerIndex);

        // Simply get the state of the controller from XInput.
        dwResult = XInputGetState(controllerIndex, &state);
        if (dwResult == ERROR_SUCCESS) {
            // Controller is connected
            newController->isConnected = true;
            XINPUT_GAMEPAD *pad        = &state.Gamepad;

            // Thumb sticks
            newController->stickLX =
                Win32HandleStickInput(pad->sThumbLX, pad->sThumbLY,
                                      XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
            newController->stickRY =
                Win32HandleStickInput(pad->sThumbRX, pad->sThumbRY,
                                      XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

            // Triggers
            // TODO: Handle trigger dead zone processing
            // pad->bLeftTrigger;
            // pad->bRightTrigger;

            // D-Pad
            // TODO: Disable D-pad stick override if playing in 3D
            if (pad->wButtons & XINPUT_GAMEPAD_DPAD_UP) {
                newController->stickLY = 1.0f;
            }
            if (pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
                newController->stickLY = -1.0f;
            }
            if (pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
                newController->stickLX = -1.0f;
            }
            if (pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
                newController->stickLX = 1.0f;
            }

            // Gamepad buttons
            Win32ProcessXInputDigitalButton(
                pad->wButtons, &oldController->actionDown,
                &newController->actionDown, XINPUT_GAMEPAD_A);
            Win32ProcessXInputDigitalButton(
                pad->wButtons, &oldController->actionRight,
                &newController->actionRight, XINPUT_GAMEPAD_B);
            Win32ProcessXInputDigitalButton(
                pad->wButtons, &oldController->actionLeft,
                &newController->actionLeft, XINPUT_GAMEPAD_X);
            Win32ProcessXInputDigitalButton(
                pad->wButtons, &oldController->actionUp,
                &newController->actionUp, XINPUT_GAMEPAD_Y);
            Win32ProcessXInputDigitalButton(
                pad->wButtons, &oldController->rightShoulder,
                &newController->rightShoulder, XINPUT_GAMEPAD_RIGHT_SHOULDER);
            Win32ProcessXInputDigitalButton(
                pad->wButtons, &oldController->leftShoulder,
                &newController->leftShoulder, XINPUT_GAMEPAD_LEFT_SHOULDER);

            Win32ProcessXInputDigitalButton(
                pad->wButtons, &oldController->start, &newController->start,
                XINPUT_GAMEPAD_START);
            Win32ProcessXInputDigitalButton(pad->wButtons, &oldController->back,
                                            &newController->back,
                                            XINPUT_GAMEPAD_BACK);

            UpdateInputState();
        } else {
            // Controller is not connected
            newController->isConnected = false;
            // TODO: Add logging
        }
    }
}

void Win32ProcessKeyboardMessage(game_button_state *newState, bool isDown)
{
    // Input is out of sync if the following assertion is true
    Assert(newState->endedDown != isDown);
    newState->endedDown = isDown;
    ++newState->halfTransitionCount;
}

void Win32ProcessXInputDigitalButton(DWORD              XInputButtonState,
                                     game_button_state *oldState,
                                     game_button_state *newState,
                                     DWORD              buttonBit)
{
    newState->endedDown = ((XInputButtonState & buttonBit) == buttonBit);
    newState->halfTransitionCount =
        (oldState->endedDown != newState->endedDown) ? 1 : 0;
}

static float Win32HandleStickInput(float x, float y, SHORT inputDeadZone)
{
    // determine how far the controller is pushed
    float magnitude = sqrt(x * x + y * y);

    // determine the direction the controller is pushed
    float normalizedLX        = x / magnitude;
    float normalizedLY        = y / magnitude;

    float normalizedMagnitude = 0;

    // check if the controller is outside a circular dead zone
    if (magnitude > inputDeadZone) {
        // clip the magnitude at its expected maximum value
        if (magnitude > 32767)
            magnitude = 32767;

        // adjust magnitude relative to the end of the dead zone
        magnitude -= inputDeadZone;

        // optionally normalize the magnitude with respect to its
        // expected range giving a magnitude value of 0.0 to 1.0
        normalizedMagnitude = magnitude / (32767 - inputDeadZone);
    } else // if the controller is in the deadzone zero out the
           // magnitude
    {
        magnitude           = 0.0;
        normalizedMagnitude = 0.0;
    }

    return normalizedMagnitude;
}

void Win32HandleKeyInput(MSG *message)
{
    uint32                 VKCode        = (uint32)message->wParam;
    bool                   wasDown       = ((message->lParam & (1 << 30)) != 0);
    bool                   isDown        = ((message->lParam & (1 << 31)) == 0);
    int                    altKeyWasDown = (message->lParam & (1 << 29));

    game_controller_input *oldKeyboard   = GetController(oldInput, 0);
    game_controller_input *newKeyboard   = GetController(newInput, 0);
    *newKeyboard                         = {};

    for (int buttonIndex = 0; buttonIndex < sizeof(newKeyboard->buttons) /
                                                sizeof(newKeyboard->buttons[0]);
         ++buttonIndex) {

        newKeyboard->buttons[buttonIndex].endedDown =
            oldKeyboard->buttons[buttonIndex].endedDown;
    }

    if (wasDown != isDown) {
        switch (VKCode) {
            case 'W':
                OutputDebugStringA("W\n");
                break;
            case 'A':
                OutputDebugStringA("A\n");
                break;
            case 'S':
                OutputDebugStringA("S\n");
                break;
            case 'D':
                OutputDebugStringA("D\n");
                break;
            case 'Q':
                Win32ProcessKeyboardMessage(&newKeyboard->leftShoulder, isDown);
                break;
            case 'E':
                Win32ProcessKeyboardMessage(&newKeyboard->rightShoulder,
                                            isDown);
                break;
            case VK_UP:
                Win32ProcessKeyboardMessage(&newKeyboard->moveUp, isDown);
                break;
            case VK_DOWN:
                Win32ProcessKeyboardMessage(&newKeyboard->moveDown, isDown);
                break;
            case VK_LEFT:
                Win32ProcessKeyboardMessage(&newKeyboard->moveLeft, isDown);
                break;
            case VK_RIGHT:
                Win32ProcessKeyboardMessage(&newKeyboard->moveRight, isDown);
                break;
            case VK_ESCAPE:
            {
                Win32ProcessKeyboardMessage(&newKeyboard->start, isDown);

                OutputDebugStringA("ESCAPE: ");
                if (isDown) {
                    OutputDebugStringA("IsDown ");
                }
                if (wasDown) {
                    OutputDebugStringA("WasDown");
                }
                OutputDebugStringA("\n");
                break;
            }
            case VK_BACK:
                Win32ProcessKeyboardMessage(&newKeyboard->back, isDown);
                break;
            case VK_SPACE:
                break;

            case VK_F4:
                if (altKeyWasDown) {
                    Running = false;
                }
                break;
        }
        // TODO: Map action buttons
    }

    UpdateInputState();
}

static void UpdateInputState()
{
    game_input *temp = newInput;
    newInput         = oldInput;
    oldInput         = temp;
}

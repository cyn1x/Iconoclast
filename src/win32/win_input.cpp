#include "win_input.h"
#include "types.h"
#include "win_window.h"

X_INPUT_GET_STATE(XInputGetStateStub) { return ERROR_DEVICE_NOT_CONNECTED; }

X_INPUT_SET_STATE(XInputSetStateStub) { return ERROR_DEVICE_NOT_CONNECTED; }

static x_input_get_state *XInputGetState_ = XInputGetStateStub;
static x_input_set_state *XInputSetState_ = XInputSetStateStub;

static platform_input     input[2];
static platform_input    *nextInput = &input[0];
static platform_input    *prevInput = &input[1];

void                      Win32HandleStickInput(XINPUT_GAMEPAD            *pad,
                                                platform_controller_input *prevControllerInput,
                                                platform_controller_input *nextControllerInput);

void Win32ProcessXInputDigitalButton(DWORD                  xInputButtonState,
                                     platform_button_state *prevState,
                                     platform_button_state *nextState,
                                     DWORD                  buttonBit);

void Win32LoadXInput(void)
{
    HMODULE xInputLibrary = LoadLibraryA("xinput1_4.dll");
    if (!xInputLibrary) {
        xInputLibrary = LoadLibraryA("xinput9_1_0.dll");
    }

    if (!xInputLibrary) {
        xInputLibrary = LoadLibraryA("xinput1_3.dll");
    }

    if (xInputLibrary) {
        XInputGetState = (x_input_get_state *)GetProcAddress(xInputLibrary,
                                                             "XInputGetState");
        if (!XInputGetState) {
            XInputGetState = XInputGetStateStub;
        }

        XInputSetState = (x_input_set_state *)GetProcAddress(xInputLibrary,
                                                             "XInputSetState");
        if (!XInputSetState) {
            XInputSetState = XInputSetStateStub;
        }
    }
}

void Win32UpdateInput(platform_input *input)
{
    DWORD dwResult;
    int   maxControllerCount = XUSER_MAX_COUNT;
    // TODO: Set maximum controller count to what is defined in input if
    // XUSER_MAX_COUNT exceeds what input supports.

    for (DWORD controllerIndex = 0; controllerIndex < maxControllerCount;
         controllerIndex++) {
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));

        platform_controller_input *prevControllerInput =
            &input->controllers[controllerIndex];
        platform_controller_input *nextControllerInput =
            &input->controllers[controllerIndex];

        // Simply get the state of the controller from XInput.
        dwResult = XInputGetState(controllerIndex, &state);
        if (dwResult == ERROR_SUCCESS) {
            // Controller is connected
            XINPUT_GAMEPAD *pad   = &state.Gamepad;

            bool            up    = (pad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
            bool            down  = (pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
            bool            left  = (pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
            bool            right = (pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
            bool            start = (pad->wButtons & XINPUT_GAMEPAD_START);
            bool            back  = (pad->wButtons & XINPUT_GAMEPAD_BACK);
            bool leftThumb        = (pad->wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
            bool rightThumb   = (pad->wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);
            bool leftShoulder = (pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
            bool rightShoulder =
                (pad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
            bool  gamepadA = (pad->wButtons & XINPUT_GAMEPAD_A);
            bool  gamepadB = (pad->wButtons & XINPUT_GAMEPAD_B);
            bool  gamepadC = (pad->wButtons & XINPUT_GAMEPAD_X);
            bool  gamepadD = (pad->wButtons & XINPUT_GAMEPAD_Y);

            int16 stick_x  = pad->sThumbLX;
            int16 stick_y  = pad->sThumbLY;

            Win32ProcessXInputDigitalButton(
                pad->wButtons, &prevControllerInput->down,
                &nextControllerInput->down, XINPUT_GAMEPAD_A);
            Win32ProcessXInputDigitalButton(
                pad->wButtons, &prevControllerInput->right,
                &nextControllerInput->right, XINPUT_GAMEPAD_B);
            Win32ProcessXInputDigitalButton(
                pad->wButtons, &prevControllerInput->left,
                &nextControllerInput->left, XINPUT_GAMEPAD_X);
            Win32ProcessXInputDigitalButton(
                pad->wButtons, &prevControllerInput->up,
                &nextControllerInput->up, XINPUT_GAMEPAD_Y);
            Win32ProcessXInputDigitalButton(pad->wButtons,
                                            &prevControllerInput->up,
                                            &nextControllerInput->rightShoulder,
                                            XINPUT_GAMEPAD_RIGHT_SHOULDER);
            Win32ProcessXInputDigitalButton(pad->wButtons,
                                            &prevControllerInput->up,
                                            &nextControllerInput->leftShoulder,
                                            XINPUT_GAMEPAD_LEFT_SHOULDER);

            Win32HandleStickInput(pad, prevControllerInput,
                                  nextControllerInput);

            platform_input *temp = nextInput;
            nextInput            = prevInput;
            prevInput            = temp;
        } else {
            // Controller is not connected
            // TODO: Add logging
        }
    }
}

void Win32ProcessXInputDigitalButton(DWORD                  xInputButtonState,
                                     platform_button_state *prevState,
                                     platform_button_state *nextState,
                                     DWORD                  buttonBit)
{
    nextState->endedDown = ((xInputButtonState & buttonBit) == buttonBit);
    nextState->halfTransitionCount =
        (prevState->endedDown != nextState->endedDown) ? 1 : 0;
}

void Win32HandleStickInput(XINPUT_GAMEPAD            *pad,
                           platform_controller_input *prevControllerInput,
                           platform_controller_input *nextControllerInput)
{
    nextControllerInput->isAnalog = true;
    nextControllerInput->startX   = prevControllerInput->endX;
    nextControllerInput->startY   = prevControllerInput->endY;
    float x;
    if (pad->sThumbLX < 0) {
        x = (float)pad->sThumbLX / 32768.0f;
    } else {
        x = (float)pad->sThumbLX / 32767.0f;
    }
    nextControllerInput->minX     = nextControllerInput->maxX =
        nextControllerInput->endX = x;

    float y;
    if (pad->sThumbLX < 0) {
        y = (float)pad->sThumbLY / 32768.0f;
    } else {
        y = (float)pad->sThumbLY / 32767.0f;
    }
    nextControllerInput->minY     = nextControllerInput->maxY =
        nextControllerInput->endY = y;
}

void Win32HandleKeyInput(WPARAM wParam, LPARAM lParam)
{
    uint32 VKCode        = wParam;
    bool   wasDown       = ((lParam & (1 << 30)) != 0);
    bool   isDown        = ((lParam & (1 << 31)) == 0);
    int    altKeyWasDown = (lParam & (1 << 29));

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
            break;
        case 'E':
            break;
        case VK_UP:
            break;
        case VK_DOWN:
            break;
        case VK_LEFT:
            break;
        case VK_RIGHT:
            break;
        case VK_ESCAPE:
        {
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
        case VK_SPACE:
            break;

        case VK_F4:
            if (altKeyWasDown) {
                Running = false;
            }
            break;
        }
    }
}

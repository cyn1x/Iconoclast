#include "input.h"

X_INPUT_GET_STATE(XInputGetStateStub)
{
    return 0;
}

X_INPUT_SET_STATE(XInputSetStateStub)
{
    return 0;
}

static x_input_get_state *XInputGetState_ = XInputGetStateStub;
static x_input_set_state *XInputSetState_ = XInputSetStateStub;

#define XInputGetState XInputGetState_
#define XInputSetState XInputSetState_

void Win32LoadXInput(void)
{
    HMODULE XInputLibrary = LoadLibraryA("xinput1_3.dll");
    if (XInputLibrary) {
        XInputGetState = (x_input_get_state *)GetProcAddress(XInputLibrary, "XInputGetState");
        if (!XInputGetState) {
            XInputGetState = XInputGetStateStub;
        }

        XInputSetState = (x_input_set_state *)GetProcAddress(XInputLibrary, "XInputSetState");
        if (!XInputSetState) {
            XInputSetState = XInputSetStateStub;
        }
    }
}

void HandleInput()
{
    DWORD dwResult;

    for (DWORD i = 0; i < XUSER_MAX_COUNT; i++) {
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));
        
        // Simply get the state of the controller from XInput.
        dwResult = XInputGetState(i, &state);
        if (dwResult == ERROR_SUCCESS) {
            // Controller is connected
            XINPUT_GAMEPAD *pad = &state.Gamepad;

            bool up = (pad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
            bool down = (pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
            bool left = (pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
            bool right = (pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
            bool start = (pad->wButtons & XINPUT_GAMEPAD_START);
            bool back = (pad->wButtons & XINPUT_GAMEPAD_BACK);
            bool left_thumb = (pad->wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
            bool right_thumb = (pad->wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);
            bool left_shoulder =
                (pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
            bool right_shoulder =
                (pad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
            bool gamepad_a = (pad->wButtons & XINPUT_GAMEPAD_A);
            bool gamepad_b = (pad->wButtons & XINPUT_GAMEPAD_B);
            bool gamepad_c = (pad->wButtons & XINPUT_GAMEPAD_X);
            bool gamepad_d = (pad->wButtons & XINPUT_GAMEPAD_Y);

            int16_t stick_x = pad->sThumbLX;
            int16_t stick_y = pad->sThumbLY;
        } else {
            // Controller is not connected
        }
    }
}

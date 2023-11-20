#include "input.h"
#include "window.h"
#include <stdint.h>

X_INPUT_GET_STATE(XInputGetStateStub)
{
    return ERROR_DEVICE_NOT_CONNECTED;
}

X_INPUT_SET_STATE(XInputSetStateStub)
{
    return ERROR_DEVICE_NOT_CONNECTED;
}

static x_input_get_state *XInputGetState_ = XInputGetStateStub;
static x_input_set_state *XInputSetState_ = XInputSetStateStub;

#define XInputGetState XInputGetState_
#define XInputSetState XInputSetState_

void Win32LoadXInput(void)
{
    HMODULE XInputLibrary = LoadLibraryA("xinput1_4.dll");
    if (!XInputLibrary) {
        XInputLibrary = LoadLibraryA("xinput1_3.dll");
    }

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

void HandleXInput()
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

void HandleKeyInput(WPARAM wParam, LPARAM lParam)
{
    uint32_t VKCode = wParam;
    bool wasDown = ((lParam & (1 << 30)) != 0);
    bool isDown = ((lParam & (1 << 31)) == 0);
    int altKeyWasDown = (lParam & (1 << 29));

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

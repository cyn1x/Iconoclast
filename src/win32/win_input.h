#ifndef WIN_INPUT_H
#define WIN_INPUT_H

// clang-format off
#include <windows.h>
#include <Xinput.h>
// clang-format on

#define X_INPUT_GET_STATE(name)                                                \
    DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE *pState)
#define X_INPUT_SET_STATE(name)                                                \
    DWORD WINAPI name(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration)

#define XInputGetState XInputGetState_
#define XInputSetState XInputSetState_

typedef X_INPUT_GET_STATE(x_input_get_state);
typedef X_INPUT_SET_STATE(x_input_set_state);

void Win32LoadXInput(void);
void HandleXInput(void);
void HandleKeyInput(WPARAM wParam, LPARAM lParam);

#endif // !WIN_INPUT_H

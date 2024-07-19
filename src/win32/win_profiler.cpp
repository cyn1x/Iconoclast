#include "win_profiler.h"
#include <stdio.h>

void Win32StartProfiler(win32_profiler *prof)
{
    QueryPerformanceFrequency(&prof->Frequency);
    QueryPerformanceCounter(&prof->StartingTime);
    prof->LastCycleCount = __rdtsc();
}

void Win32UpdateProfiler(win32_profiler *prof)
{
    // Calculates the elapsed miliseconds per frame, the frames per second and
    // the megacycles per frame.
    prof->EndCycleCount = __rdtsc();
    QueryPerformanceCounter(&prof->EndingTime);

    prof->ElapsedMicroseconds.QuadPart =
        prof->EndingTime.QuadPart - prof->StartingTime.QuadPart;

    float millisecondsElapsed = 1000.0f *
                                (float)prof->ElapsedMicroseconds.QuadPart /
                                (float)prof->Frequency.QuadPart;
    float cyclesElapsed =
        (float)prof->EndCycleCount - (float)prof->LastCycleCount;
    float framesPerSecond = (float)prof->Frequency.QuadPart /
                            (float)prof->ElapsedMicroseconds.QuadPart;

    prof->ElapsedMicroseconds.QuadPart *= 1000;
    prof->ElapsedMicroseconds.QuadPart /= prof->Frequency.QuadPart;

    char buf[256];
    sprintf(buf, "%.2f ms/f: %.2f f/s: %.2f mc/f\n", millisecondsElapsed,
            framesPerSecond, cyclesElapsed / (1000.0f * 1000.0f));
    OutputDebugStringA(buf);

    prof->LastCycleCount = prof->EndCycleCount;
    prof->StartingTime   = prof->EndingTime;
}

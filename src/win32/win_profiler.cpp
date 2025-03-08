#include "win_profiler.h"
#include <stdio.h>

void Win32StartProfiler(win32_profiler *profiler)
{
    QueryPerformanceFrequency(&profiler->Frequency);
    QueryPerformanceCounter(&profiler->StartingTime);
    profiler->LastCycleCount = __rdtsc();
}

void Win32UpdateProfiler(win32_profiler *profiler)
{
    // Calculates the elapsed miliseconds per frame, the frames per second and
    // the megacycles per frame.
    profiler->EndCycleCount = __rdtsc();
    QueryPerformanceCounter(&profiler->EndingTime);

    profiler->ElapsedMicroseconds.QuadPart =
        profiler->EndingTime.QuadPart - profiler->StartingTime.QuadPart;

    float millisecondsElapsed = 1000.0f *
                                (float)profiler->ElapsedMicroseconds.QuadPart /
                                (float)profiler->Frequency.QuadPart;
    float secondsElapsed = millisecondsElapsed / 1000.0f;
    float cyclesElapsed =
        (float)profiler->EndCycleCount - (float)profiler->LastCycleCount;
    float framesPerSecond = (float)profiler->Frequency.QuadPart /
                            (float)profiler->ElapsedMicroseconds.QuadPart;

    profiler->LastCycleCount = profiler->EndCycleCount;
    profiler->StartingTime   = profiler->EndingTime;

    profiler->ElapsedMicroseconds.QuadPart *= 100000;
    profiler->ElapsedMicroseconds.QuadPart /= profiler->Frequency.QuadPart;
    profiler->ElapsedMilliseconds = millisecondsElapsed;
    profiler->ElapsedSeconds      = secondsElapsed;

    char buf[256];
    sprintf_s(buf, "%.2f ms/f: %.2f f/s: %.2f mc/f\n", millisecondsElapsed,
              framesPerSecond, cyclesElapsed / (1000.0f * 1000.0f));
    OutputDebugStringA(buf);
}

double Win32GetCurrentTime(win32_profiler *profiler)
{
    LARGE_INTEGER result;
    QueryPerformanceCounter(&result);
    return result.QuadPart / (double)profiler->Frequency.QuadPart * 1000.0f;
}

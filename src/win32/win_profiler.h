#ifndef WIN_PROFILER_H
#define WIN_PROFILER_H

#include <windows.h>

typedef struct win32_profiler
{
    LARGE_INTEGER    StartingTime, EndingTime;
    LARGE_INTEGER    ElapsedMicroseconds, ElapsedCycles;
    LARGE_INTEGER    Frequency;
    unsigned __int64 LastCycleCount, EndCycleCount;
} win32_profiler;

void Win32StartProfiler(win32_profiler *prof);
void Win32UpdateProfiler(win32_profiler *prof);
void Win32EndProfiler(win32_profiler *prof);

#endif // !WIN_PROFILER_H

#ifndef WIN_PROFILER_H
#define WIN_PROFILER_H

#include <windows.h>

typedef struct profiler
{
    LARGE_INTEGER    StartingTime, EndingTime;
    LARGE_INTEGER    ElapsedMicroseconds, ElapsedCycles;
    LARGE_INTEGER    Frequency;
    unsigned __int64 LastCycleCount, EndCycleCount;
} profiler;

void Win32StartProfiler(profiler *prof);
void Win32UpdateProfiler(profiler *prof);
void Win32EndProfiler(profiler *prof);

#endif // !WIN_PROFILER_H

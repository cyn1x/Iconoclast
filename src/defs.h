#ifndef DEFS_H
#define DEFS_H

#if DEBUG
#define Assert(Expression)                                                     \
    if (!(Expression)) {                                                       \
        *(int *)0 = 0;                                                         \
    }
#else
#define Assert(Expression)
#endif

#define Kilobytes(Bytes) ((Bytes) * 1024LL)
#define Megabytes(Bytes) (Kilobytes(Bytes) * 1024LL)
#define Gigabytes(Bytes) (Megabytes(Bytes) * 1024LL)
#define Terabytes(Bytes) (Gigabytes(Bytes) * 1024LL)

#endif // !DEFS_H

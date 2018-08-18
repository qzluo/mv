#ifndef PUBLIC_H
#define PUBLIC_H

#define GETBIT(x, b)   ((x&(1<<b))?1:0)
#define SETBIT(x, b)   (x|=1<<b)
#define CLRBIT(x, b)   (x&=~(1<<b))

#define NUMTOASCII(num)     (((num) < 10) ? ('0' + (num)) : ('A' + (num) - 10))

#define ISNUM(c)            (((c) >= '0' && (c) <= '9') || \
    (((c) >= 'a' && (c) <= 'f') || ((c) >= 'A' && (c) <= 'F')) ? 1 : 0)

#define ASCIITONUM(c)       (((c) >= '0' && (c) <= '9') ? ((c) - '0') : \
    ((c) >= 'a' && (c) <= 'f') ? ((c) - 'a' + 10) : \
    ((c) >= 'A' && (c) <= 'F') ? ((c) - 'A' + 10) : (c))

#endif // PUBLIC_H

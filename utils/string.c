#include "string.h"

int strlen(const char *str)
{
    int len = 0;
    while (str[len])
        len++;
    return len;
}

int strcmp(const char *a, const char *b)
{
    while (*a && (*a == *b)) {
        a++;
        b++;
    }
    return (unsigned char)*a - (unsigned char)*b;
}

int strncmp(const char *a, const char *b, int n)
{
    while (n && *a && (*a == *b)) {
        a++;
        b++;
        n--;
    }

    if (n == 0)
        return 0;

    return (unsigned char)*a - (unsigned char)*b;
}

void *memcpy(void *dest, const void *src, int n)
{
    unsigned char *d = dest;
    const unsigned char *s = src;

    while (n--)
        *d++ = *s++;

    return dest;
}

void *memset(void *dest, int val, int n)
{
    unsigned char *d = dest;

    while (n--)
        *d++ = (unsigned char)val;

    return dest;
}

int memcmp(const void *a, const void *b, int n)
{
    const unsigned char *p1 = a;
    const unsigned char *p2 = b;

    while (n--) {
        if (*p1 != *p2)
            return *p1 - *p2;
        p1++;
        p2++;
    }

    return 0;
}


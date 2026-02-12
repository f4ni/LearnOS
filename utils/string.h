#ifndef STRING_H
#define STRING_H

int strlen(const char *str);
int strcmp(const char *a, const char *b);
int strncmp(const char *a, const char *b, int n);

void *memcpy(void *dest, const void *src, int n);
void *memset(void *dest, int val, int n);
int memcmp(const void *a, const void *b, int n);

#endif
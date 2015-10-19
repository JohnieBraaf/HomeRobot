#pragma once
#include <assert.h>

static void _strdelchr(char*, size_t, size_t*, size_t*);
size_t strdelstr(char*, const char*);
char * substr(char*, int, int);
char * substrdelim(char*, char*);

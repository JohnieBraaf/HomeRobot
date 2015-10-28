#pragma once
#include <assert.h>

static void _strdelchr(char*, size_t, size_t*, size_t*);
size_t strdelstr(char*, const char*);
char *substr(char*, int, int);
char *substrdelim(char*, char*);
char *substrdelim2(char*, char*, char*);
char** str_split(char*, const char);
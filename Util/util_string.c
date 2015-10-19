#include <HomeRobot.h>

/*
 * delete one character from a string
 */
static void _strdelchr(char *s, size_t i, size_t *a, size_t *b)
{
	size_t        j;

	if (*a == *b)
		*a = i - 1;
	else
		for (j = *b + 1; j < i; j++)
			s[++(*a)] = s[j];
	*b = i;
}

/*
 * delete all occurrences of characters in search from s
 * returns nr. of deleted characters
 */
size_t strdelstr(char *s, const char *search)
{ 
	size_t        l               = strlen(s);
	size_t        n               = strlen(search);
	size_t        i;
	size_t        a               = 0;
	size_t        b               = 0;

	for (i = 0; i < l; i++)
		if (memchr(search, s[i], n))
			_strdelchr(s, i, &a, &b);
	_strdelchr(s, l, &a, &b);
	s[++a] = '\0';
	return l - a;
}

/*
 * get a substring
 */
char * substr(char * s, int x, int y)
{
	char * ret = malloc(strlen(s) + 1);
	char * p = ret;
	char * q = &s[x];

	assert(ret != NULL);

	while (x  < y)
	{
		*p++ = *q++;
		x++; 
	}

	*p++ = '\0';

	return ret;
}

/*
 * get a substring based on a the first and second occurance of a delimiter
 */
char *substrdelim(char *string, char *delimiter)
{
	if (*string != NULL)
	{
		int index1 = 0, index2 = 0, cnt = 0;
		for (int i = 0; string[i] != '\0'; i++) {
			if (strncmp(&string[i], delimiter, 1) == 0)
			{
				if (cnt == 0) 
				{
					index1 = i;
					cnt++;
				}
				else {
					index2 = i;	
					break;
				}
			}
		}
		
		if (index2 > 0)
		{
			return substr(string, index1, index2);
		}
	}
	return "";
}
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
 * get a substring based on the first and second occurance of a delimiter
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

/*
 * get a substring based on a the first and second delimiter
 */
char *substrdelim2(char *string, char *delimiter1, char *delimiter2)
{
	if (*string != NULL)
	{
		int index1 = 0, index2 = 0, cnt = 0;
		for (int i = 0; string[i] != '\0'; i++) {
			if (strncmp(&string[i], delimiter1, 1) == 0 && cnt == 0)
			{
				index1 = i;
				cnt++;
				if (index2 > 0)
					break;
			}
			
			else if (strncmp(&string[i], delimiter2, 1) == 0 && index2 == 0)
			{
				index2 = i;
				if (cnt > 0)
					break;
			}
		}
		
		if (index2 > index1)
		{
			int num = index2 - index1 - 1;
			char str[num + 1];

			for (int i = 0; i < num; i++)
			{
				str[i] = string[index1 + 1 + i];//substr(string, index1, index2);
			}
			str[num] = '\0';
			return strdup(str);
		}
	}
	return "";
}

/*
 * Split a string based on a delimiter
 */
char** str_split(char* a_str, const char a_delim)
{
	char** result    = 0;
	size_t count     = 0;
	char* tmp        = a_str;
	char* last_comma = 0;
	char delim[2];
	delim[0] = a_delim;
	delim[1] = 0;

	    /* Count how many elements will be extracted. */
	while (*tmp)
	{
		if (a_delim == *tmp)
		{
			count++;
			last_comma = tmp;
		}
		tmp++;
	}

	    /* Add space for trailing token. */
	count += last_comma < (a_str + strlen(a_str) - 1);

	    /* Add space for terminating null string so caller
	       knows where the list of returned strings ends. */
	count++;

	result = malloc(sizeof(char*) * count);

	if (result)
	{
		size_t idx  = 0;
		char* token = strtok(a_str, delim);

		while (token)
		{
			assert(idx < count);
			*(result + idx++) = strdup(token);
			token = strtok(0, delim);
		}
		assert(idx == count - 1);
		*(result + idx) = 0;
	}

	return result;
}
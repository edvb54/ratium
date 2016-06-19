#ifndef UTIL_H
#define UTIL_H

#define LEN(X) (sizeof(X) / sizeof((X)[0]))

int estrcmp(const char *s1, const char *s2);
char  *strdup(const char *s);
char **str_split(const char *s, const char a_delim);
int count_chars(char *s, char ch);

#endif

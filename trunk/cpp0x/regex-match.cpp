#include <cstdio>

/* match: search for regexp anywhere in text */
int match(const char *regexp, const char *text);
int matchhere(const char *regexp, const char *text);
int matchstar(int c, const char *regexp, const char *text);

constexpr int match_c(const char *regexp, const char *text);
constexpr int matchhere_c(const char *regexp, const char *text);
constexpr int matchstar_c(int c, const char *regexp, const char *text);

int match(const char *regexp, const char *text)
{
    if (regexp[0] == '^')
        return matchhere(regexp+1, text);
    do {    /* must look even if string is empty */
        if (matchhere(regexp, text))
            return 1;
    } while (*text++ != '\0');
    return 0;
}

/* matchhere: search for regexp at beginning of text */
int matchhere(const char *regexp, const char *text)
{
    if (regexp[0] == '\0')
        return 1;
    if (regexp[1] == '*')
        return matchstar(regexp[0], regexp+2, text);
    if (regexp[0] == '$' && regexp[1] == '\0')
        return *text == '\0';
    if (*text!='\0' && (regexp[0]=='.' || regexp[0]==*text))
        return matchhere(regexp+1, text+1);
    return 0;
}

/* matchstar: search for c*regexp at beginning of text */
int matchstar(int c, const char *regexp, const char *text)
{
    do 
    {    /* a * matches zero or more instances */
        if (matchhere(regexp, text))
            return 1;
    } while (*text != '\0' && (*text++ == c || c == '.'));
    return 0;
}

constexpr int match_c(const char *regexp, const char *text)
{
    return (regexp[0] == '^') ? matchhere(regexp+1, text) :

    do {    /* must look even if string is empty */
        if (matchhere(regexp, text))
            return 1;
    } while (*text++ != '\0');
    return 0;
}

/* matchhere: search for regexp at beginning of text */
int matchhere(const char *regexp, const char *text)
{
    if (regexp[0] == '\0')
        return 1;
    if (regexp[1] == '*')
        return matchstar(regexp[0], regexp+2, text);
    if (regexp[0] == '$' && regexp[1] == '\0')
        return *text == '\0';
    if (*text!='\0' && (regexp[0]=='.' || regexp[0]==*text))
        return matchhere(regexp+1, text+1);
    return 0;
}

/* matchstar: search for c*regexp at beginning of text */
int matchstar(int c, const char *regexp, const char *text)
{
    do 
    {    /* a * matches zero or more instances */
        if (matchhere(regexp, text))
            return 1;
    } while (*text != '\0' && (*text++ == c || c == '.'));
    return 0;
}

#define TO_STR_IMPL(R) #R
#define TO_STR(R) TO_STR_IMPL(R)

int main(void)
{
  printf("match = %d\n", match(TO_STR(REGEX), TO_STR(TEXT)));

  return 0;
}


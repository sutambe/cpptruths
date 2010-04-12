#include <stdio.h>
#include <string.h>

int pathendcmp(const char *, const char *);
int main(void)
{
    char *src="/home/trutta2/srg/projects/dynamic-loader/sumant-tambe/redirect-lib/libwrap2.so.0.0";
    char *needle="libwrap2.so.0.0";
    int found;

    found=pathendcmp(src,needle);
    printf("Found: %d\n",found);

    return 0;
}
int pathendcmp(const char *s1, const char *s2)
{
    const char *s1_ptr;
    const char *s2_ptr;
    int retval=0;

    //if(0==*s1 || 0==*s2) return -999;

    s1_ptr = s1 + strlen(s1)-1;
    s2_ptr = s2 + strlen(s2)-1;

    while(s1 <= s1_ptr && s2 <= s2_ptr && (*s1_ptr == *s2_ptr))
    { --s1_ptr; --s2_ptr; };

    if(s2 > s2_ptr)
    {
        if(s1 > s1_ptr) retval=0;
        else if('/'==*s1_ptr) retval=0;
        else retval=-999;
    }
    else if(s1 > s1_ptr)
    {
        if(s2 > s2_ptr) retval=0;
        else if('/'==*s2_ptr) retval=0;
        else retval=-999;
    }
    else retval=-999;

    return retval;
}

/*
int strcmpend(const char *src, const char *needle)
{
    int needle_length;
    const char *src_end;

    if(!*needle || !*src) return -999;

    needle_length=strlen(needle);
    src_end=src + strlen(src);
    return strcmp(src_end-needle_length,needle);
}
*/

#include <stdio.h>

int main()
{
static int i=5;

if(i--)
main();

        printf ("%d\n",i);
        return 0;
}

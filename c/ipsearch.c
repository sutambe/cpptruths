#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char command[1000]={0};
    int i,j;
    for(i=64;i < 65;i++)
    {
        for(j=0;j <= 255;j++)
        {
            sprintf(command,"wget http://128.123.%d.%d --timeout=2 --tries=1 -nv",i,j);
            puts(command);
            system(command);
        }
    }
    
    return 0;
}

/*
1. make it 'n--' instead of 'i--'
2. make it 'i+n' instead of 'i<n'
3. make it '-i<n' instead of 'i<n'
4. make it 'i<-n' instead of 'i<n'
*/

#include <stdio.h>
#include <string.h>

int func(int []);
int main(void)
{
    int i,n=20;
    int pqr[20];
    char ch[]="abcd";
    i=i;
    printf("size of ch = %d, strlen = %d\n",sizeof(ch),strlen(ch));
    func(pqr);
    
    for(int p=0;-p <? n;n--)
    {
	/*printf("-");*/
    }
    
    
    
    return 0;

}

int func(int abc[7])
{
	printf("%d\n",sizeof(abc));
	abc++;
	return 0;
}

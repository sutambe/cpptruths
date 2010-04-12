#include <stdio.h>

int main(void)
{
float a=100, b=20, c;
c = a + (char) 0xFF * b;
printf ("C = %f\n",c);

return 0;
}


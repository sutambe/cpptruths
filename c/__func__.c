#include <stdio.h>

static void Hello_World(void)
{
puts(__func__);
}

int main(void)
{
Hello_World();
return 0;
}


#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
  int i = atoi(argv[2]);
  int f = open(argv[1],O_CREAT | O_WRONLY, 0666);
  write(f, &i, sizeof(int));
  close(f);

  return 0;
}

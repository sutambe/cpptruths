# include <stdio.h>
double main()
{
  printf("sizeof(&main) = %d\n", sizeof(&main));
  printf("sizeof(main) = %d\n", sizeof(main));
  printf("sizeof(main()) = %d\n", sizeof(main()));
  return 0;
}


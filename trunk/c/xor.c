#define printf(...)
extern int printf (const char *,...);
int main(void)
{
  int i = 10, j = 20;
  // Folloiwng statement is ocmpiler dependent because
  // the order of expression evaluation is not defined.
  i ^= j ^= i ^= j;
  printf ("%d %d\n", i, j);
  return 0;
}


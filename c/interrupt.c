int main (void)
{
  int result, angle;
  asm ("fsinx %1,%1" 
      : "=f" (result) 
      : "f" (angle));
}

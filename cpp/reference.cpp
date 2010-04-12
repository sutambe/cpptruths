template <int n, int m, int p>
int average (int (&array)[n][m][p]) 
{  
}
int main (void)
{
  int * const & v = new int;
  //int a [] = {1, 2};
  int b [][2][2] = { { {1}, {3}} ,{{5}, {6}} };
  average (a);
  average (b);
  return 0;
}


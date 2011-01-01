#include <cstdio>

template<int Depth, int A, typename B>
struct K17 
{ 
  enum { value = 1 +
  K17<Depth+1, 0, K17>::value
+ K17<Depth+1, 1, K17>::value };

};

template<int A, typename B>
struct K17<17,A,B> 
{ 
  enum { value = 1 };
};

int main(void)
{
  const int instantiations = K17<0,0,int>::value;
  printf("%d\n",instantiations);
}


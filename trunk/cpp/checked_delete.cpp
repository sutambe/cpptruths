template<class T> 
inline void checked_delete(T * x)
{
   typedef char type_must_be_complete[ sizeof(T)? 1: -1 ];
   (void) sizeof(type_must_be_complete);
   delete x;
}

class X;

int main (void)
{
  X * p;
  checked_delete (p);

  return 0;
}

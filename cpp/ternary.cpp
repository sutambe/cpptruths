class B {}; 
class D1 : public B {}; 
class D2 : public B {}; 

D1 && __d1();
D2 && __d2();

int main(int argc, char * argv[]) 
{ 
   D1* d1 = 0; 
   D2* d2 = 0; 
   B* b_ok; 
   if (d1) b_ok = d1; 
   else    b_ok = d2; 
   B* b_ko = (d1 ? d1 : d2); // Does not compile because types of d1, d2 are not inter-convertible.
   //B* b_ko = (d1 ? __d1() : __d2()); 
}

#include <iostream>

template<typename T>
T ft() { return T(); }
/*  
template<>
int ft<int>() { return 1; }
   
template<>
float ft<float>() { return 0.5; }
  */  
struct F
{
    template<typename T>
    operator T() { return ft<T>(); }
};
 
int main()
{
    F x; 
    int il = x;
    float fl = F();
    bool b = F();
    std::cout << il << "\n" <<  fl << "\n" << b << std::endl;
}


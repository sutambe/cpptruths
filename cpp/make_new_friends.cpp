#include <iostream>

using namespace std;

// Forward declarations
template<class T> class Box;
template<class T>
ostream& operator<<(ostream&, const Box<T>&);

template <typename T>
class Person
{
  T t;
};

template<class T> class Box {
  T value;
public:
    Box(const T& t) {
        value = t;
    }
  friend ostream& operator<< <>(ostream&, const Box<T>&);
  friend class Person<T>;
};

template<class T>
ostream& operator<<(ostream& os, const Box<T>& b) {
  return os << b.value;
}

int main(void)
{
        Box <int> b (10);
        std::cout << b << std::endl;
        Box <double> c (10.345);
        std::cout << c << std::endl;
        return 0;
}


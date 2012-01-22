#include <vector>
#include <cstdio>

#define USING(Dervied, Base)                                       \
        template<typename ...Args,                                   \
             typename = typename std::enable_if                    \
             <                                                     \
                std::is_constructible<Base, Args...>::value        \
             >::type>                                              \
    Dervied(Args &&...args)                                        \
        : Base(std::forward<Args>(args)...) { }                    \

template <class T>
class MyVector : public std::vector<T>
{
  public:
    USING(MyVector, std::vector<T>)

};


int main(void)
{
  std::vector<int> v(10, 999);
  std::vector<int>::iterator iter = v.end();
  printf("%d\n", iter[-1]);

  MyVector<int> mv(4,5);

  return 0;
}

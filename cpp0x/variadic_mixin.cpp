#include <utility>

template <template<class> class... Mixins>
class Host : public Mixins<Host<Mixins...>>... 
{
  public:
    //Host(Mixins<Host>&&... args) : Mixins<Host>(std::forward<Mixins<Host>>(args))... {}
    
    template <class... Args>
    Host(Args&&... args) : Mixins<Host>(std::forward<Mixins<Host>>(args))... {}
};

template <class Host> struct Mix1 {};

template <class Host> struct Mix2 {};

int main (void)
{
  typedef Host<Mix1, Mix2> TopHost;
  delete new TopHost(Mix1<TopHost>(), Mix2<TopHost>());
}


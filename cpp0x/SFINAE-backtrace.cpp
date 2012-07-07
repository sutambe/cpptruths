template<class T> auto h(T x)->decltype(x.smurf()){return x.smurf();}
template<class T> auto g(T x)->decltype(h(x)){return h(x);}
template<class T> auto f(T x)->decltype(g(x)){return g(x);}
int main(){
  f(3);
}

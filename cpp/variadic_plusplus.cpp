void inc() { }

template<typename T, typename ... Args>
void inc(T& t, Args& ... args) { ++t; inc(args...); }

int main(void)
{
  int i, j, k;
  inc(i, j, k);
  inc();
}


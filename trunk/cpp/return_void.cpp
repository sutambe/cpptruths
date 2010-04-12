static void foo (void){
}

static void bar (void) {
  return foo(); // Note this return statement.
}

template <class T>
T FOO (void) {
  return T();
}

template <class T>
T BAR (void) {
  return FOO<T>();
}

int main (void) {
  bar();
  BAR<void>();
}

static void foo (void){
}

static void bar (void) {
  return foo(); // Note this return statement.
}

int main (void) {
  bar();
  return 0;
}

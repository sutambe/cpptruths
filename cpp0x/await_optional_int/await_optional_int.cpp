// Compile: cl /EHsc /await /O2 await_optional_int.cpp

#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <cstdio>
#include <memory>
#include <string>
#include <stdexcept>
#include <chrono>
#include <pplawait.h>
#include <experimental/generator>

//#define printf(...) 

#define MayBe await

class OptionalInt
{
  struct State { 
    int value; 
    bool valid; 
  };

  std::shared_ptr<State> state;

public:
  typedef OptionalInt promise_type;

  OptionalInt() 
    : state(std::make_shared<State>(State{ -1, false })) 
  {
    printf("created empty OptionalInt %p\n", this);
  }
  
  OptionalInt(int value) 
    : state(std::make_shared<State>(State{ value, true })) 
  {
    printf("created OptionalInt = %d, %p\n", value, this);
  }
  
  OptionalInt get_return_object() { 
    printf("get_return_object\n");
    return *this; 
  }
  
  bool initial_suspend() { return false; }
  
  bool final_suspend() { return false; }

  void set_exception(std::exception_ptr e) {
    state->valid = false;
    printf("got exception!\n");
  }
  
  void return_value(int i) {
    state->value = i;
    state->valid = true;
    printf("return_value = %d\n", i);
  }
  
  int get() const { 
    return state->valid ? state->value : throw std::runtime_error("Empty OptionalInt"); 
  }

  bool valid() const { 
    return state->valid;
  }

  bool await_ready() {
    printf("inside await_ready %p\n", this);
    return valid();
  }
  
  void await_suspend(std::experimental::coroutine_handle<OptionalInt> rh) {
      printf("inside await_suspend %p\n", this);
      rh.destroy(); // to avoid memory leak
      printf("suspend complete %p\n", this);
  } 

  int await_resume() {
      printf("inside await_resume %p\n", this);
      return get();
  }
};

OptionalInt quadruple(int i)
{
  printf("inside quadruple\n");
  return (i % 2) ? OptionalInt(i*4) : OptionalInt();
}

OptionalInt testOptionalInt()
{
  static int i = 0;
  i++;
  
  auto v1 = MayBe quadruple(i);
  printf("v1 = %d\n\n", v1);
  auto v2 = MayBe quadruple(v1);
  printf("v2 = %d\n\n", v2);
  
  return v2+55;
}

OptionalInt testIfThenElse()
{
  static int i = 0;
  i++;
  
  auto v1 = quadruple(i);
  if(v1.valid())
  {
    printf("%d\n", v1.get());
    auto v2 = quadruple(v1.get());
    if(v2.valid())
    {
      printf("%d\n", v2.get());
      return OptionalInt(55);
    }
  }
  return OptionalInt();
}

int add(int i, int j, int k) { return i+j+k; }

OptionalInt testParallel()
{
  return add(await quadruple(1), 
             await quadruple(3),
             await quadruple(5));
}

int main(int argc, char * argv[])
{
  try {
    int iter = 10000;
    
    if (argv[1])
      iter = atoi(argv[1]);
    
    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::microseconds microseconds;
    Clock::time_point t0, t1;
    microseconds us;
    
    t0 = Clock::now();    
    for(int i = 1;i <= iter; i++)
    {
      auto x = testOptionalInt();
      if (x.valid())
        printf("x = %d\n", x.get());
      else
        printf("invalid\n");
    }
    t1 = Clock::now();    
    us = std::chrono::duration_cast<microseconds>(t1 - t0);
    
    if(0)
      fprintf(stdout, "testOptionalInt time = %llu microseconds\n", us.count());
    
    if (0)
    {
      t0 = Clock::now();
      for (int i = 1; i <= iter; i++)
      {
        auto x = testIfThenElse();
        if (x.valid())
          printf("x = %d", x.get());
        else
          printf("invalid\n");
      }
      t1 = Clock::now();
      us = std::chrono::duration_cast<microseconds>(t1 - t0);

      fprintf(stdout, "testIfThenElse time = %llu microseconds\n", us.count());
    }

    new int[5]{ 0xFF }; // deliberate leak
    
    if(0)
      fprintf(stdout, "testParallel = %d\n", testParallel().get());
  }
  catch(std::exception & ex)
  {
     fprintf(stdout, "main: exception = %s", ex.what());
  }

  _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_WNDW);
  _CrtDumpMemoryLeaks();
  getchar();
}
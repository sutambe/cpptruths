#include <string>
#include <stdexcept>
#include <iostream>

class UndoString : public std::string
{
  struct proxy
  {
    UndoString * str;
    size_t pos;

    proxy(UndoString * us, size_t position)
      : str(us), pos(position)
    {}

    void operator = (const char & rhs)
    {
      str->old = str->at(pos);
      str->old_pos = pos;
      str->at(pos) = rhs;
    }
    
    operator const char & () const
    {
      return str->at(pos);
    }
    
    operator char & ()
    {
      str->old = str->at(pos);
      str->old_pos = pos;
      return str->at(pos);
    }
  };

  char old;
  int old_pos;

public:

  UndoString(const std::string & s)
    : std::string(s), old(0), old_pos(-1)
  {}

  proxy operator [] (size_t index)
  {
    return proxy(this, index);
  }
  
  using std::string::operator [];

  void undo()
  {
    if(old_pos == -1)
      throw std::runtime_error("Nothing to undo!");

    std::string::at(old_pos) = old;
    old = 0;
    old_pos = -1;
  }
};

void modify(char & c)
{
  c = 'Z';
}

int main(void)
{
  UndoString ub("More C++ Idioms");
  std::cout << ub[0];  // Prints 'M'
  ub[0] = 'm';         // Change 'M' to 'm'
  std::cout << ub[0];  // Prints 'm'
  ub.undo();           // Restore'M'
  std::cout << ub[0];  // Prints 'M'
  modify(ub[0]);
  std::cout << ub[0]; 
  //ub.undo();
}

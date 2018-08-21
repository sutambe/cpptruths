#include <iostream>
#include <iomanip>

#define UNARY_LEFT_FOLD(NAME, OP)   \
template<typename... Args>          \
auto NAME(Args&&... args) {         \
  return (... OP args);             \
}

UNARY_LEFT_FOLD(add,+);
UNARY_LEFT_FOLD(sub,-);
UNARY_LEFT_FOLD(mul,*);
UNARY_LEFT_FOLD(divide,/);
UNARY_LEFT_FOLD(mod,%);
UNARY_LEFT_FOLD(bxor,^);
UNARY_LEFT_FOLD(band,&);
UNARY_LEFT_FOLD(bor,|);
UNARY_LEFT_FOLD(assign,=);
UNARY_LEFT_FOLD(lt,<);
#ifndef __clang__ 
UNARY_LEFT_FOLD(gt,>); 
UNARY_LEFT_FOLD(rshift,>>); 
#endif
UNARY_LEFT_FOLD(lshift,<<);
UNARY_LEFT_FOLD(addassign,+=);
UNARY_LEFT_FOLD(subassign,-=);
UNARY_LEFT_FOLD(mulassign,*=);
UNARY_LEFT_FOLD(divassign,/=);
UNARY_LEFT_FOLD(modassign,%=);
UNARY_LEFT_FOLD(bxorassign,^=);
UNARY_LEFT_FOLD(bandassign,&=);
UNARY_LEFT_FOLD(borassign,|=);
UNARY_LEFT_FOLD(lshiftassign,<<=);
UNARY_LEFT_FOLD(rshiftassign,>>=);
UNARY_LEFT_FOLD(equals,==);
UNARY_LEFT_FOLD(nequals,!=);
UNARY_LEFT_FOLD(lte,<=);
UNARY_LEFT_FOLD(gte,>=);
UNARY_LEFT_FOLD(land,&&);
UNARY_LEFT_FOLD(lor,||);
UNARY_LEFT_FOLD(objptrmem,.*);
UNARY_LEFT_FOLD(ptrptrmem,->*);

template<typename... Args>
auto comma(Args&&... args) {
  return (... , args);
}

struct Phone  { int ext; };
struct Person { Phone phone;  };

int main(void) 
{
  std::cout << std::boolalpha;
  std::cout << "add "            << add(1)           << " " << add(1,2,3)        << "\n";
  std::cout << "sub "            << sub(1)           << " " << sub(1,2,3)        << "\n";
  std::cout << "mul "            << mul(1)           << " " << mul(1,2,3)        << "\n";
  std::cout << "divide "         << divide(4)        << " " << divide(18,2,3)    << "\n";
  std::cout << "mod "            << mod(5)           << " " << mod(23, 3,2)      << "\n";
  std::cout << "bxor "           << bxor(4)          << " " << bxor(1,2,4)       << "\n";
  std::cout << "band "           << band(5)          << " " << band(1,3,7)       << "\n";
  std::cout << "assign "         << assign(7)        << " " << assign(1,2,4)     << "\n";
    
  auto a = 99; 
  std::cout << "assign-a "       << assign(a);
  std::cout << " "               << assign(a,2,4);
  std::cout << " "               << a << "\n";
    
  #ifndef __clang__ 
  std::cout << "gt "             << gt(4,2)          << " " << gt(3,2,0)         << "\n"; 
  std::cout << "rshift "         << rshift(4)        << " " << rshift(32,2,2)    << "\n"; 
  #endif

  std::cout << "lt "             << lt(5)            << " " << lt(1,2,-1)         << "\n"; 
  std::cout << "lshift "         << lshift(5)        << " " << lshift(1,2,3)     << "\n";
  std::cout << "addassign "      << addassign(5)     << " " << addassign(2,3,2)  << "\n";
  std::cout << "subassign "      << subassign(4)     << " " << subassign(7,2)    << "\n";
  std::cout << "mulassign "      << mulassign(5)     << " " << mulassign(2,3,2)  << "\n";
  std::cout << "divassign "      << divassign(4)     << " " << divassign(7,2)    << "\n";
  std::cout << "modassign "      << modassign(5)     << " " << modassign(23,3,2) << "\n";
  std::cout << "bxorassign "     << bxorassign(4)    << " " << bxorassign(7,2)   << "\n";
  std::cout << "bandassign "     << bandassign(4)    << " " << bandassign(7,6)   << "\n";
  std::cout << "borassign "      << borassign(5)     << " " << borassign(1,2,4,8) << "\n";
  std::cout << "lshiftassign "   << lshiftassign(4)  << " " << lshiftassign(8,2)  << "\n";
  std::cout << "rshiftassign "   << rshiftassign(4)  << " " << rshiftassign(16,1,2)   << "\n";
  std::cout << "equals "         << equals(5)        << " " << equals(8,3,2)     << "\n";
  std::cout << "nequals "        << nequals(4)       << " " << nequals(7,2,0)    << "\n";
  std::cout << "lte "            << lte(4)           << " " << lte(7,2,0)        << "\n";
  std::cout << "gte "            << gte(5)           << " " << gte(7,3,1)        << "\n";
  std::cout << "land "           << land(4)          << " " << land(7,2)         << "\n";
  std::cout << "lor "            << lor(4)           << " " << lor(7,2)          << "\n";
  std::cout << "comma "          << comma(5)         << " " << comma(8,3,2)      << "\n";
  
  auto phoneptr = &Person::phone;
  auto extptr = &Phone::ext;
  Person p { { 999 } };
  Person * pptr = &p;
  std::cout << "objptrmem "                   << objptrmem(p,phoneptr,extptr)       << "\n";
  std::cout << "p.*phoneptr.*extptr "         << p.*phoneptr.*extptr                << "\n";
  std::cout << "ptrptrmem(&p,phoneptr).ext "  << ptrptrmem(&p,phoneptr).ext         << "\n";  
  std::cout << "&(pptr->*phoneptr)->*extptr " << (&(pptr->*phoneptr))->*extptr      << "\n";

}

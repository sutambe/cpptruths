#include <iostream>

struct S
{
  bool val;

  explicit S(int i) : val(i) {}  
  explicit S(bool b) : val(b) {}
  
  template <class Expr>
  S (const Expr & expr)
   : val(evaluate(expr).val)
  { }

  template <class Expr>
  S & operator = (const Expr & expr)
  {
    val = evaluate(expr).val;
  }
    
  explicit operator bool () const 
  {
    return val;
  }

  S logical_and (const S & rhs) const
  {
    std::cout << "&& ";
    return S{val && rhs.val};
  }
  
  S logical_or (const S & rhs) const
  {
    std::cout << "|| ";
    return S{val || rhs.val};
  }
};

const S & evaluate(const S &s) 
{
  return s;
}

template <class Expr>
S evaluate(const Expr & expr) 
{
  return expr.eval();
}

struct And 
{
  template <class LExpr, class RExpr>
  S operator ()(const LExpr & l, const RExpr & r) const
  {
    const S & temp = evaluate(l);
    return temp? temp.logical_and(evaluate(r)) : temp;
  }
};

struct Or 
{
  template <class LExpr, class RExpr>
  S operator ()(const LExpr & l, const RExpr & r) const
  {
    const S & temp = evaluate(l);
    return temp? temp : temp.logical_or(evaluate(r));
  }
};


template <class Op, class LExpr, class RExpr>
struct Expr
{
  Op op;
  const LExpr &lhs;
  const RExpr &rhs;
  
  Expr(const LExpr& l, const RExpr & r)
   : lhs(l),
     rhs(r)
  {}
  
  S eval() const 
  {
    return op(lhs, rhs);
  }
};

template <class LExpr>
Expr<And, LExpr, S> operator & (const LExpr & lhs, const S & rhs)
{
  return Expr<And, LExpr, S> (lhs, rhs);
}

template <class LExpr, class Op, class L, class R>
Expr<And, LExpr, Expr<Op,L,R>> operator & (const LExpr & lhs, const Expr<Op,L,R> & rhs)
{
  return Expr<And, LExpr, Expr<Op,L,R>> (lhs, rhs);
}

template <class LExpr>
Expr<Or, LExpr, S> operator | (const LExpr & lhs, const S & rhs)
{
  return Expr<Or, LExpr, S> (lhs, rhs);
}

template <class LExpr, class Op, class L, class R>
Expr<Or, LExpr, Expr<Op,L,R>> operator | (const LExpr & lhs, const Expr<Op,L,R> & rhs)
{
  return Expr<Or, LExpr, Expr<Op,L,R>> (lhs, rhs);
}

std::ostream & operator << (std::ostream & o, const S & s)
{
  o << s.val;
  return o;
}

S and_result(S s1, S s2, S s3)
{
  return s1 & s2 & s3;
}

S or_result(S s1, S s2, S s3)
{
  return s1 | s2 | s3;
}

int main(void) 
{
  for(int i=0; i<= 1; ++i)
    for(int j=0; j<= 1; ++j)
      for(int k=0; k<= 1; ++k)
        std::cout << and_result(S{i}, S{j}, S{k}) << std::endl;
  
  for(int i=0; i<= 1; ++i)
    for(int j=0; j<= 1; ++j)
      for(int k=0; k<= 1; ++k)
        std::cout << or_result(S{i}, S{j}, S{k}) << std::endl;
  
  return 0;
}

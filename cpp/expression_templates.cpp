#include <iostream>
#include <vector>
#include <algorithm>

struct Var {
	double operator () (double v) { return v; }
};

struct Constant {
		double c;
		Constant (double d) : c (d) {}
		double operator () (double) { return c; }
};

template <class L, class H, class OP>
struct DBinExp {
	L l_;
	H h_;
	DBinExp (L l, H h) : l_ (l), h_ (h) {}
	double operator () (double d) { return OP::apply (l_ (d), h_(d)); }
};

struct Add {
	static double apply (double l, double h) { return l + h; }
};

template <class Itr, class Func>
void evaluate (Itr begin, Itr end, Func func) 
{
	for (Itr i = begin; i != end; ++i)
		std::cout << func (*i) << std::endl;
}

template <class E>
struct DExpr {
      E expr_;
      DExpr (E e) : expr_ (e) {}
	  double operator() (double d) { return expr_(d);  }
};

template<class A, class B>
DExpr<DBinExp<DExpr<A>, DExpr<B>, Add> >
operator + (DExpr<A> a, DExpr<B> b)
{
  typedef DBinExp <DExpr<A>, DExpr<B>, Add> ExprT;
  return DExpr<ExprT>(ExprT(a,b));
}

int main (void)
{
	typedef DExpr <Var> Variable;
	typedef DExpr <Constant> Literal;
	typedef DBinExp <Variable , Literal, Add> VarLitAdder;
	typedef DExpr <VarLitAdder> MyAdder;

	Variable v ((Var()));
	Literal l (Constant (50.00));
	VarLitAdder vl_adder(v, l);
	MyAdder expr (vl_adder);

	std::vector <double> a;
	a.push_back (10);
	a.push_back (20);

	evaluate (a.begin(), a.end(), expr + expr + expr);

	return 0;
}
